/*-------------------------------------------------------------------------------------------------------------------------------------------
 * ps2kbd.c - PS/2 keyboard functions
 *-------------------------------------------------------------------------------------------------------------------------------------------
 * See also:
 *   https://www.marjorie.de/ps2/ps2_keyboard.htm
 *   https://www.marjorie.de/ps2/ps2_protocol.htm
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
 */
#include "stdint.h"

#include "stm32f10x_conf.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_dma.h"

#include "delay.h"
#include "io.h"
#include "ps2kbd.h"

#define PS2_PERIPH_CLOCK_PORT   RCC_APB2Periph_GPIOB
#define PS2_CLOCK_PORT          GPIOB
#define PS2_CLOCK_PIN           GPIO_Pin_12

#define PS2_PERIPH_DATA_PORT    RCC_APB2Periph_GPIOB
#define PS2_DATA_PORT           GPIOB
#define PS2_DATA_PIN            GPIO_Pin_13

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * ps2kbd_clock_low() - set clock pin to LOW
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
ps2kbd_clock_low ()
{
    GPIO_RESET_BIT (PS2_CLOCK_PORT, PS2_CLOCK_PIN);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * ps2kbd_clock_high() - set clock pin to HIGH
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
ps2kbd_clock_high ()
{
    GPIO_SET_BIT (PS2_CLOCK_PORT, PS2_CLOCK_PIN);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * ps2kbd_data_low() - set data pin to LOW
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
ps2kbd_data_low ()
{
    GPIO_RESET_BIT (PS2_DATA_PORT, PS2_DATA_PIN);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * ps2kbd_data_high() - set data pin to HIGH
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
ps2kbd_data_high ()
{
    GPIO_SET_BIT (PS2_DATA_PORT, PS2_DATA_PIN);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * ps2kbd_send_bit () - send bit
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
ps2kbd_send_bit (uint_fast8_t bitval)
{
    if (bitval)
    {
        ps2kbd_data_high ();
    }
    else
    {
        ps2kbd_data_low ();
    }

    delay_usec (15);
    ps2kbd_clock_low ();
    delay_usec (30);
    ps2kbd_clock_high ();
    delay_usec (15);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * ps2kbd_send_code () - send PS/2 code
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
ps2kbd_send_code (uint_fast8_t ch)
{
    uint_fast8_t    bit;
    uint_fast8_t    odd_parity = 1;

    ps2kbd_send_bit (0);                    // send start bit

    for (bit = 0; bit < 8; bit++)
    {
        if (ch & 0x01)
        {
            odd_parity++;
            ps2kbd_send_bit (0x01);         // send data bit 1
        }
        else
        {
            ps2kbd_send_bit (0x00);         // send data bit 0
        }
        ch >>= 1;
    }

    ps2kbd_send_bit (odd_parity & 0x01);    // send odd parity bit
    ps2kbd_send_bit (1);                    // send stop bit
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * ps2kbd_init() - initialize PS/2
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
ps2kbd_init (void)
{
    GPIO_InitTypeDef gpio;

    GPIO_StructInit (&gpio);
    RCC_APB2PeriphClockCmd (PS2_PERIPH_CLOCK_PORT, ENABLE);
    gpio.GPIO_Pin      = PS2_CLOCK_PIN;
    gpio.GPIO_Mode     = GPIO_Mode_Out_OD;
    gpio.GPIO_Speed    = GPIO_Speed_2MHz;
    GPIO_Init(PS2_CLOCK_PORT, &gpio);
    ps2kbd_clock_high ();

    GPIO_StructInit (&gpio);
    RCC_APB2PeriphClockCmd (PS2_PERIPH_DATA_PORT, ENABLE);
    gpio.GPIO_Pin      = PS2_DATA_PIN;
    gpio.GPIO_Mode     = GPIO_Mode_Out_OD;
    gpio.GPIO_Speed    = GPIO_Speed_2MHz;
    GPIO_Init(PS2_DATA_PORT, &gpio);
    ps2kbd_data_high ();
}
