/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * main.c - main functions of zxkeyboard
 *-------------------------------------------------------------------------------------------------------------------------------------------
 * MIT License
 *
 * Copyright (c) 2021 Frank Meyer - frank(at)fli4l.de
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 * Internal devices used on STM32F103C8T6 Mini Development Board:
 *
 *    +-------------------------+------------------------------------+-------------------------------+
 *    | Device                  | STM32F103 Mini Development Board   | Remarks                       |
 *    |-------------------------|------------------------------------|-------------------------------|
 *    | Board LED               | GPIO          PC13                 | lit when key pressed          |
 *    |-------------------------|------------------------------------|-------------------------------|
 *    | Keyboard row 0          | GPIO          PA0                  | Z80 A8  - Caps Shift - V      |
 *    | Keyboard row 1          | GPIO          PA1                  | Z80 A9  - A          - G      |
 *    | Keyboard row 2          | GPIO          PA2                  | Z80 A10 - Q          - T      |
 *    | Keyboard row 3          | GPIO          PA3                  | Z80 A11 - 1          - 5      |
 *    | Keyboard row 4          | GPIO          PA4                  | Z80 A12 - 0          - 6      |
 *    | Keyboard row 5          | GPIO          PA5                  | Z80 A13 - P          - Y      |
 *    | Keyboard row 6          | GPIO          PA6                  | Z80 A14 - ENTER      - H      |
 *    | Keyboard row 7          | GPIO          PA7                  | Z80 A15 - SPACE      - B      |
 *    | Keyboard col 0          | GPIO          PB3                  | Z80 D0                        |
 *    | Keyboard col 1          | GPIO          PB4                  | Z80 D1                        |
 *    | Keyboard col 2          | GPIO          PB5                  | Z80 D2                        |
 *    | Keyboard col 3          | GPIO          PB6                  | Z80 D3                        |
 *    | Keyboard col 4          | GPIO          PB7                  | Z80 D4                        |
 *    | Keyboard col 5          | GPIO          PB8                  | extra column for menu key     |
 *    | Communication with Pi   | UART3 TX      PB10  (38400 Bd)     | UART (optional)               |
 *    | Communication with Pi   | UART3 RX      PB11  (38400 Bd)     | UART (optional)               |
 *    | Communication with F407 | GPIO          PB12                 | PS/2 Clock                    |
 *    | Communication with F407 | GPIO          PB13                 | PS/2 Data                     |
 *    +-------------------------+------------------------------------+-------------------------------+
 *
 *                       Columns                                   Rows
 *               ---  ---  ---  ---  ---            ---  ---  ---  ---  ---  ---  ---  ---
 *   Z80         D0   D1   D2   D3   D4             A11  A10  A9   A12  A13  A8   A14  A15
 *   STM32F103   PB3  PB4  PB5  PB6  PB7            PA3  PA2  PA1  PA4  PA5  PA0  PA6  PA7
 *
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#include <stdint.h>
#include <string.h>
#include "delay.h"
#include "board-led.h"
#include "zxkbd.h"
#include "serial.h"
#include "ps2kbd.h"

static const uint16_t       scancodes[ZX_KBD_ROWS][ZX_KBD_EXT_COLS] =
{   //      D0                      D1                     D2                  D3                  D4                 D5 (extra col)
    {   PS2KBD_SCANCODE_LSHFT,  PS2KBD_SCANCODE_Z,     PS2KBD_SCANCODE_X,  PS2KBD_SCANCODE_C,  PS2KBD_SCANCODE_V,  PS2KBD_SCANCODE_TAB  },
    {   PS2KBD_SCANCODE_A,      PS2KBD_SCANCODE_S,     PS2KBD_SCANCODE_D,  PS2KBD_SCANCODE_F,  PS2KBD_SCANCODE_G,         0             },
    {   PS2KBD_SCANCODE_Q,      PS2KBD_SCANCODE_W,     PS2KBD_SCANCODE_E,  PS2KBD_SCANCODE_R,  PS2KBD_SCANCODE_T,         0             },
    {   PS2KBD_SCANCODE_1,      PS2KBD_SCANCODE_2,     PS2KBD_SCANCODE_3,  PS2KBD_SCANCODE_4,  PS2KBD_SCANCODE_5,         0             },
    {   PS2KBD_SCANCODE_0,      PS2KBD_SCANCODE_9,     PS2KBD_SCANCODE_8,  PS2KBD_SCANCODE_7,  PS2KBD_SCANCODE_6,         0             },
    {   PS2KBD_SCANCODE_P,      PS2KBD_SCANCODE_O,     PS2KBD_SCANCODE_I,  PS2KBD_SCANCODE_U,  PS2KBD_SCANCODE_Y,         0             },
    {   PS2KBD_SCANCODE_ENTER,  PS2KBD_SCANCODE_L,     PS2KBD_SCANCODE_K,  PS2KBD_SCANCODE_J,  PS2KBD_SCANCODE_H,         0             },
    {   PS2KBD_SCANCODE_SPACE,  PS2KBD_SCANCODE_LCTRL, PS2KBD_SCANCODE_M,  PS2KBD_SCANCODE_N,  PS2KBD_SCANCODE_B,         0             },
};

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
int
main (void)
{
    uint_fast8_t    row;
    uint_fast8_t    col;
    uint_fast8_t    state;
    uint16_t        ps2key_scancode;
    uint32_t        delay_value;

    SystemInit ();
    SystemCoreClockUpdate ();

#if defined (STM32F103)                                                     // disable JTAG to get back PB3, PB4, PA13, PA14, PA15
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_AFIO, ENABLE);                   // turn on clock for the alternate function register
    GPIO_PinRemapConfig (GPIO_Remap_SWJ_JTAGDisable, ENABLE);               // disable JTAG, enable SWD
#endif

    delay_init (DELAY_RESOLUTION_5_US);
    board_led_init ();
    serial_init (38400);
    ps2kbd_init ();
    zxkbd_init ();

    while (1)
    {
        for (row = 0; row < ZX_KBD_ROWS; row++)
        {
            delay_value = 4000;                                             // debounce: 8 x 4000 usec = 32 msec

            zxkbd_io (row);                                                 // read columns of row

            if (zxkbd_row_changed (row))
            {
                for (col = 0; col < ZX_KBD_EXT_COLS; col++)
                {
                    state = zxkbd_key_state (row, col);

                    if (state != ZXKBD_KEY_NOCHANGE)
                    {
                        ps2key_scancode = scancodes[row][col];

                        if (ps2key_scancode)
                        {
                            if (ps2key_scancode & PS2KBD_EXTENDED_FLAG)
                            {
                                serial_putc (0xE0);                         // send extend code per UART
                                ps2kbd_send_code (0xE0);                    // send extend code per PS/2

                                if (delay_value > 330)
                                {
                                    delay_value -= 330;                     // sending code per PS/2 takes 11 x 30usec = 330usec
                                }
                            }

                            if (state == ZXKBD_KEY_RELEASED)                // key released?
                            {
                                serial_putc (0xF0);                         // send break code per UART
                                ps2kbd_send_code (0xF0);                    // send break code per PS/2

                                if (delay_value > 330)
                                {
                                    delay_value -= 330;                     // sending code per PS/2 takes 11 x 30usec = 330usec
                                }
                            }

                            serial_putc (ps2key_scancode & 0xFF);           // send 8 bit scancode per UART
                            ps2kbd_send_code (ps2key_scancode & 0xFF);      // send 8 bit scancode per PS/2

                            if (delay_value > 330)
                            {
                                delay_value -= 330;                         // sending code per PS/2 takes 11 x 30usec = 330usec
                            }
                        }
                    }
                }
            }

            delay_usec (delay_value);                                       // debounce: 8 x 4000 usec = 32 msec
        }
    }
}
