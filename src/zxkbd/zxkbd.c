/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * zxkbd.c - ZX keyboard functions
 *-------------------------------------------------------------------------------------------------------------------------------------------
 * ZX Keyboard matrix: 8 lines, 5 columns
 *
 *          Z80  D0 D1 D2 D3 D4   D4 D3 D2 D1 D0  Z80
 *  row 3 - A11  1  2  3  4  5    6  7  8  9  0   A12 - row 4
 *  row 2 - A10  Q  W  E  R  T    Y  U  I  O  P   A13 - row 5
 *  row 1 -  A9  A  S  D  F  G    H  J  K  L  CR  A14 - row 6
 *  row 0 -  A8  CS Z  X  C  V    B  N  M  SS SP  A15 - row 7
 *
 *  Keyboard connector on original Spectrum board:
 *
 *              ---  ---  ---  ---  ---            ---  ---  ---  ---  ---  ---  ---  ---
 *  Z80         D0   D1   D2   D3   D4             A11  A10  A9   A12  A13  A8   A14  A15
 *  STM32       PB3  PB4  PB5  PB6  PB7            PA3  PA2  PA1  PA4  PA5  PA0  PA6  PA7
 *
 *  Additional button at PB8 and PA3 - Menu Key
 *------------------------------------------------------------------------------------------------------------------------
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
 */
#include <stdio.h>
#include <string.h>

#include "stm32f10x_conf.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_dma.h"

#include "delay.h"
#include "board-led.h"
#include "zxkbd.h"

static uint8_t              zxkbd_matrix[ZX_KBD_ROWS];                          // keyboard matrix: 0 = pressed, 1 = released
static uint8_t              last_zxkbd_matrix[ZX_KBD_ROWS];                     // last state of keyboard matrix

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * zxkbd_init() - initialize kbd port
 *
 * PA0 - PA7      8 output lines, open drain
 * PB3 - PB8      6 input columns, with pullup
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
zxkbd_init (void)
{
    GPIO_InitTypeDef gpio;

    GPIO_StructInit (&gpio);
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);

    gpio.GPIO_Pin      = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    gpio.GPIO_Mode     = GPIO_Mode_Out_OD;
    gpio.GPIO_Speed    = GPIO_Speed_2MHz;

    GPIO_Init(GPIOA, &gpio);
    GPIOA->BSRR = 0x00FF;                                   // set bits 0 - 7

    GPIO_StructInit (&gpio);
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB, ENABLE);

    gpio.GPIO_Pin      = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
    gpio.GPIO_Mode     = GPIO_Mode_IPU;
    gpio.GPIO_Speed    = GPIO_Speed_2MHz;

    GPIO_Init(GPIOB, &gpio);

    memset (last_zxkbd_matrix, ZX_KBD_EXT_COLMASK, sizeof (last_zxkbd_matrix));
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * zxkbd_io () - read keyboard row
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
zxkbd_io (uint_fast8_t row)
{
    uint8_t key;

    last_zxkbd_matrix[row] = zxkbd_matrix[row];             // save last state

    GPIOA->BRR  = 1 << row;                                 // reset one bit corresponding to addr
    delay_usec (15);                                        // wait 15usec until signals are stable
    key = GPIOB->IDR >> 3;                                  // read port B, shift lower 3 bits
    GPIOA->BSRR = 0x00FF;                                   // set bits 0 - 7 again
    zxkbd_matrix[row] = key & ZX_KBD_EXT_COLMASK;           // store lower 6 bits (5 cols + 1 extra col), 0 = key pressed, 1 = key released
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * zxkbd_row_changed () - check if row changed
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
zxkbd_row_changed (uint_fast8_t row)
{
    uint_fast8_t    rtc;

    if (zxkbd_matrix[row] != last_zxkbd_matrix[row])
    {
        rtc = 1;
    }
    else
    {
        rtc = 0;
    }

    return rtc;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * zxkbd_key_state () - return state of a row/column
 *
 * Return values:
 *  ZXKBD_KEY_NOCHANGE          - no change
 *  ZXKBD_KEY_PRESSED           - key pressed
 *  ZXKBD_KEY_RELEASED          - key released
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
zxkbd_key_state (uint_fast8_t row, uint_fast8_t col)
{
    uint_fast8_t    colmask;
    uint_fast8_t    state;

    colmask = 1 << col;

    if ((zxkbd_matrix[row] & colmask) != (last_zxkbd_matrix[row] & colmask))
    {
        if (zxkbd_matrix[row] & colmask)
        {
            board_led_off ();
            state = ZXKBD_KEY_RELEASED;
        }
        else
        {
            board_led_on ();
            state = ZXKBD_KEY_PRESSED;
        }
    }
    else
    {
        state = ZXKBD_KEY_NOCHANGE;
    }

    return state;
}
