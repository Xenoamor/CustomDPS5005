/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#include <stdio.h>
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx.h"
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "spi.h"
#include "timer.h"
#include "usart.h"
#include "gpio.h"
#include <stdbool.h>
#include "ili9163c.h"

static void LL_Init(void);
static void System_Clock_Init(void);

/**
  * @brief      The application entry point.
  *
  * @return     None
  */
int main(void)
{
    // Reset all peripherals, Initializes the Flash interface and the Systick.
    LL_Init();

    // Configure the system clock
    System_Clock_Init();

    // Initialize all configured peripherals
    GPIO_Init();
    USART_Init();
    SPI_Init();
    DAC_Init();
    ADC_Init();
    TIM4_Init();

    ili9163c_init();

    /* Main loop */
    while (1)
    {
        bool m1 = LL_GPIO_IsInputPinSet(GPIO_BUTTON_M1_GPIO_Port, GPIO_BUTTON_M1_Pin);
        bool m2 = LL_GPIO_IsInputPinSet(GPIO_BUTTON_M2_GPIO_Port, GPIO_BUTTON_M2_Pin);
        bool sel = LL_GPIO_IsInputPinSet(GPIO_BUTTON_SEL_GPIO_Port, GPIO_BUTTON_SEL_Pin);

        bool power = LL_GPIO_IsInputPinSet(GPIO_BUTTON_POWER_GPIO_Port, GPIO_BUTTON_POWER_Pin);
        bool rotPress = LL_GPIO_IsInputPinSet(GPIO_BUTTON_ROT_PRESS_GPIO_Port, GPIO_BUTTON_ROT_PRESS_Pin);
        bool rotA = LL_GPIO_IsInputPinSet(GPIO_BUTTON_ROT_A_GPIO_Port, GPIO_BUTTON_ROT_A_Pin);
        bool rotB = LL_GPIO_IsInputPinSet(GPIO_BUTTON_ROT_B_GPIO_Port, GPIO_BUTTON_ROT_B_Pin);

        printf("m1 %d m2 %d sel %d power %d rotPress %d rotA %d rotB %d\r", m1, m2, sel, power, rotPress, rotA, rotB);

        //uint16_t i_out_raw, v_in_raw, v_out_raw;
        //ADC_get_values(&i_out_raw, &v_in_raw, &v_out_raw);
        //printf("ADC values: v_in: %05u, i_out: %05u, v_out: %05u\r", v_in_raw, i_out_raw, v_out_raw);
    }
}

static void LL_Init(void)
{
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* System interrupt init*/
    /* MemoryManagement_IRQn interrupt configuration */
    NVIC_SetPriority(MemoryManagement_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    /* BusFault_IRQn interrupt configuration */
    NVIC_SetPriority(BusFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    /* UsageFault_IRQn interrupt configuration */
    NVIC_SetPriority(UsageFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    /* SVCall_IRQn interrupt configuration */
    NVIC_SetPriority(SVCall_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    /* DebugMonitor_IRQn interrupt configuration */
    NVIC_SetPriority(DebugMonitor_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    /* PendSV_IRQn interrupt configuration */
    NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    /* SysTick_IRQn interrupt configuration */
    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));

    /* DISABLE: JTAG-DP Disabled and SW-DP Disabled */
    LL_GPIO_AF_DisableRemap_SWJ();
}

/**
  * @brief      System Clock Configuration
  */
static void System_Clock_Init(void)
{
    LL_RCC_HSI_SetCalibTrimming(16);

    LL_RCC_HSI_Enable();

    /* Wait till HSI is ready */
    while (LL_RCC_HSI_IsReady() != 1)
    {}

    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_6);

    LL_RCC_PLL_Enable();

    /* Wait till PLL is ready */
    while (LL_RCC_PLL_IsReady() != 1)
    {}

    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

    /* Wait till System clock is ready */
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    {}

    LL_Init1msTick(CPU_FREQUENCY);

    LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);

    LL_SYSTICK_EnableIT();

    LL_SetSystemCoreClock(CPU_FREQUENCY);

    LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSRC_PCLK2_DIV_2);

    /* SysTick_IRQn interrupt configuration */
    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
}