/*-------------------------------------------------------------------------------------------------------------------------------------------
 * board-led.c - LED routines
 *-------------------------------------------------------------------------------------------------------------------------------------------
 * MIT License
 *
 * Copyright (c) 2019-2021 Frank Meyer - frank(at)fli4l.de
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
#if defined (STM32F10X)
#include "stm32f10x.h"
#elif defined (STM32F4XX)
#include "stm32f4xx.h"
#endif
#include "board-led.h"
#include "io.h"

#if defined (DISCOVERY_BOARD_STM32F407VG)                                                       // STM32F4 Discovery Board: PD12
#define BOARD_LED_PERIPH_CLOCK_CMD  RCC_AHB1PeriphClockCmd
#define BOARD_LED_PERIPH            RCC_AHB1Periph_GPIOD
#define BOARD_LED_PORT              GPIOD
#define BOARD_LED_LED               GPIO_Pin_12
#define BOARD_LED_ON                GPIO_SET_BIT(BOARD_LED_PORT, BOARD_LED_LED)                 // active high
#define BOARD_LED_OFF               GPIO_RESET_BIT(BOARD_LED_PORT, BOARD_LED_LED)

#elif defined (BLACK_BOARD_STM32F407VE)                                                         // STM32F407VE Black Board: PA6
#define BOARD_LED_PERIPH_CLOCK_CMD  RCC_AHB1PeriphClockCmd
#define BOARD_LED_PERIPH            RCC_AHB1Periph_GPIOA
#define BOARD_LED_PORT              GPIOA
#define BOARD_LED_LED               GPIO_Pin_6
#define BOARD_LED_ON                GPIO_RESET_BIT(BOARD_LED_PORT, BOARD_LED_LED)               // active low
#define BOARD_LED_OFF               GPIO_SET_BIT(BOARD_LED_PORT, BOARD_LED_LED)

#elif defined (NUCLEO_BOARD_STM32F4XX)                                                          // STM32F4xx Nucleo Board: PA5
#define BOARD_LED_PERIPH_CLOCK_CMD  RCC_AHB1PeriphClockCmd
#define BOARD_LED_PERIPH            RCC_AHB1Periph_GPIOA
#define BOARD_LED_PORT              GPIOA
#define BOARD_LED_LED               GPIO_Pin_5
#define BOARD_LED_ON                GPIO_SET_BIT(BOARD_LED_PORT, BOARD_LED_LED)                 // active high
#define BOARD_LED_OFF               GPIO_RESET_BIT(BOARD_LED_PORT, BOARD_LED_LED)

#elif defined (BLUEPILL_BOARD_STM32F103)                                                        // STM32F103 BluePill (Mini Dev) Board: PC13
#define BOARD_LED_PERIPH_CLOCK_CMD  RCC_APB2PeriphClockCmd
#define BOARD_LED_PERIPH            RCC_APB2Periph_GPIOC
#define BOARD_LED_PORT              GPIOC
#define BOARD_LED_LED               GPIO_Pin_13
#define BOARD_LED_ON                GPIO_RESET_BIT(BOARD_LED_PORT, BOARD_LED_LED)               // active low
#define BOARD_LED_OFF               GPIO_SET_BIT(BOARD_LED_PORT, BOARD_LED_LED)

#else
#error board unknown
#endif

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * initialize LED port
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
board_led_init (void)
{
    GPIO_InitTypeDef gpio;

    GPIO_StructInit (&gpio);
    BOARD_LED_PERIPH_CLOCK_CMD (BOARD_LED_PERIPH, ENABLE);     // enable clock for LED Port

    gpio.GPIO_Pin   = BOARD_LED_LED;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;

#if defined (STM32F10X)
    gpio.GPIO_Mode  = GPIO_Mode_Out_PP;
#elif defined (STM32F4XX)
    gpio.GPIO_Mode  = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;
#endif

    GPIO_Init(BOARD_LED_PORT, &gpio);
    board_led_off ();
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * green LED on
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
board_led_on (void)
{
    BOARD_LED_ON;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * green LED off
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void
board_led_off (void)
{
    BOARD_LED_OFF;
}
