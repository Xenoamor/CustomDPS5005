/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CPU_FREQUENCY (24000000)

#define GPIO_UNKNOWN_U7_2_Pin LL_GPIO_PIN_1
#define GPIO_UNKNOWN_U7_2_GPIO_Port GPIOD
#define GPIO_UNKNOWN_U7_1_Pin LL_GPIO_PIN_0
#define GPIO_UNKNOWN_U7_1_GPIO_Port GPIOA
#define GPIO_BUTTON_M2_Pin LL_GPIO_PIN_1
#define GPIO_BUTTON_M2_GPIO_Port GPIOA
#define GPIO_BUTTON_SEL_Pin LL_GPIO_PIN_2
#define GPIO_BUTTON_SEL_GPIO_Port GPIOA
#define GPIO_BUTTON_M1_Pin LL_GPIO_PIN_3
#define GPIO_BUTTON_M1_GPIO_Port GPIOA
#define GPIO_TL594_2IN_P_Pin LL_GPIO_PIN_15
#define GPIO_TL594_2IN_P_GPIO_Port GPIOA
#define GPIO_SGM8582_P_Pin LL_GPIO_PIN_3
#define GPIO_SGM8582_P_GPIO_Port GPIOB
#define GPIO_BUTTON_POWER_Pin LL_GPIO_PIN_4
#define GPIO_BUTTON_POWER_GPIO_Port GPIOB
#define GPIO_BUTTON_ROT_PRESS_Pin LL_GPIO_PIN_5
#define GPIO_BUTTON_ROT_PRESS_GPIO_Port GPIOB
#define GPIO_UNKNOWN_Pin LL_GPIO_PIN_6
#define GPIO_UNKNOWN_GPIO_Port GPIOB
#define GPIO_BUTTON_ROT_A_Pin LL_GPIO_PIN_8
#define GPIO_BUTTON_ROT_A_GPIO_Port GPIOB
#define GPIO_BUTTON_ROT_B_Pin LL_GPIO_PIN_9
#define GPIO_BUTTON_ROT_B_GPIO_Port GPIOB
#define GPIO_POWER_OUTPUT_ENABLE_Pin LL_GPIO_PIN_11
#define GPIO_POWER_OUTPUT_ENABLE_GPIO_Port GPIOB
#define GPIO_TFT_RESET_Pin LL_GPIO_PIN_12
#define GPIO_TFT_RESET_GPIO_Port GPIOB
#define GPIO_TFT_A0_Pin LL_GPIO_PIN_14
#define GPIO_TFT_A0_GPIO_Port GPIOB
#define GPIO_TFT_UNUSED_Pin LL_GPIO_PIN_8
#define GPIO_TFT_UNUSED_GPIO_Port GPIOA

#define GPIO_SWDIO_Pin LL_GPIO_PIN_13
#define GPIO_SWDIO_GPIO_Port GPIOA
#define GPIO_SWDCLK_Pin LL_GPIO_PIN_14
#define GPIO_SWDCLK_GPIO_Port GPIOA

#define DAC_OUT_V_SET_Pin LL_GPIO_PIN_4
#define DAC_OUT_V_SET_GPIO_Port GPIOA
#define DAC_OUT_I_SET_Pin LL_GPIO_PIN_5
#define DAC_OUT_I_SET_GPIO_Port GPIOA

#define ADC_I_OUT_Pin LL_GPIO_PIN_7
#define ADC_I_OUT_GPIO_Port GPIOA
#define ADC_V_IN_Pin LL_GPIO_PIN_0
#define ADC_V_IN_GPIO_Port GPIOB
#define ADC_V_OUT_Pin LL_GPIO_PIN_1
#define ADC_V_OUT_GPIO_Port GPIOB

#define SPI2_SCK_TFT_Pin LL_GPIO_PIN_13
#define SPI2_SCK_TFT_GPIO_Port GPIOB
#define SPI2_MISO_TFT_Pin LL_GPIO_PIN_14
#define SPI2_MISO_TFT_GPIO_Port GPIOB
#define SPI2_MOSI_TFT_Pin LL_GPIO_PIN_15
#define SPI2_MOSI_TFT_GPIO_Port GPIOB

#define USART1_TX_PC_Pin LL_GPIO_PIN_9
#define USART1_TX_PC_GPIO_Port GPIOA
#define USART1_RX_PC_Pin LL_GPIO_PIN_10
#define USART1_RX_PC_GPIO_Port GPIOA

#define TIM4_TFT_BACKLIGHT_Pin LL_GPIO_PIN_7
#define TIM4_TFT_BACKLIGHT_GPIO_Port GPIOB


#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */