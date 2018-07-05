/**
  ******************************************************************************
  * File Name          : DAC.c
  * Description        : This file provides code for the configuration
  *                      of the DAC instances.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_dac.h"
#include "main.h"
#include "dac.h"
#include "gpio.h"
#include "time.h"

/* DAC init function */
void DAC_Init(void)
{
    LL_DAC_InitTypeDef DAC_InitStruct;

    LL_GPIO_InitTypeDef GPIO_InitStruct;
    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_DAC1);

    /**DAC GPIO Configuration
    PA4   ------> DAC_OUT1
    PA5   ------> DAC_OUT2
    */
    GPIO_InitStruct.Pin = DAC_OUT_V_SET_Pin | DAC_OUT_I_SET_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /**DAC channel OUT1 config
    */
    DAC_InitStruct.TriggerSource = LL_DAC_TRIG_SOFTWARE;
    DAC_InitStruct.WaveAutoGeneration = LL_DAC_WAVE_AUTO_GENERATION_NONE;
    DAC_InitStruct.OutputBuffer = LL_DAC_OUTPUT_BUFFER_ENABLE;
    LL_DAC_Init(DAC, LL_DAC_CHANNEL_1, &DAC_InitStruct);

    LL_DAC_DisableTrigger(DAC, LL_DAC_CHANNEL_1);

    /**DAC channel OUT2 config
    */
    LL_DAC_Init(DAC, LL_DAC_CHANNEL_2, &DAC_InitStruct);

    LL_DAC_DisableTrigger(DAC, LL_DAC_CHANNEL_2);

    /* Enable DAC channel */
    LL_DAC_Enable(DAC1, LL_DAC_CHANNEL_1);

    /* Delay for DAC voltage to settle */
    //Time_Delay_Ms( (LL_DAC_DELAY_STARTUP_VOLTAGE_SETTLING_US / 1000) + 1 );
    Time_Delay_Ms( 1 );

    /* Start both DAC channels */
    DAC_Set_Outputs(0, 0);
}

__INLINE void DAC_Set_V_Out(uint16_t v_out_raw)
{
    LL_DAC_ConvertData12RightAligned(DAC, LL_DAC_CHANNEL_1, v_out_raw);
}

__INLINE void DAC_Set_I_Out(uint16_t i_out_raw)
{
    LL_DAC_ConvertData12RightAligned(DAC, LL_DAC_CHANNEL_2, i_out_raw);
}

__INLINE void DAC_Set_Outputs(uint16_t v_out_raw, uint16_t i_out_raw)
{
    // Load both channels simultaneously
    LL_DAC_ConvertDualData12RightAligned(DAC, v_out_raw, i_out_raw);
}