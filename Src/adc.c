/**
  ******************************************************************************
  * File Name          : ADC.c
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
  *******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_adc.h"
#include "main.h"
#include "adc.h"
#include "gpio.h"
#include "time.h"
#include <stdio.h>

static volatile uint16_t ADC_i_out_raw;
static volatile uint16_t ADC_v_out_raw;
static volatile uint16_t ADC_v_in_raw;

/* ADC1 init function */
void ADC_Init(void)
{
    LL_ADC_InitTypeDef ADC_InitStruct;
    LL_ADC_REG_InitTypeDef ADC_REG_InitStruct;
    LL_ADC_INJ_InitTypeDef ADC_INJ_InitStruct;

    LL_GPIO_InitTypeDef GPIO_InitStruct;
    /* Peripheral clock enable */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);

    /**ADC1 GPIO Configuration
    PA7   ------> ADC1_IN7
    PB0   ------> ADC1_IN8
    PB1   ------> ADC1_IN9
    */
    GPIO_InitStruct.Pin = ADC_I_OUT_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    LL_GPIO_Init(ADC_I_OUT_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ADC_V_IN_Pin | ADC_V_OUT_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* ADC1 interrupt Init */
    NVIC_SetPriority(ADC1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(ADC1_IRQn);

    LL_ADC_EnableIT_JEOS(ADC1); // Enable interrupt on ADC group injected end of sequence conversion events

    /**Common config
    */
    ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
    ADC_InitStruct.SequencersScanMode = LL_ADC_SEQ_SCAN_ENABLE;
    LL_ADC_Init(ADC1, &ADC_InitStruct);

    ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
    ADC_REG_InitStruct.SequencerLength = 1;
    ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
    ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_CONTINUOUS;
    ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
    LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);

    /**Configure Injected Channel
    */
    ADC_INJ_InitStruct.TriggerSource = LL_ADC_INJ_TRIG_SOFTWARE;
    ADC_INJ_InitStruct.SequencerLength = LL_ADC_INJ_SEQ_SCAN_ENABLE_3RANKS;
    ADC_INJ_InitStruct.SequencerDiscont = LL_ADC_INJ_SEQ_DISCONT_DISABLE;
    ADC_INJ_InitStruct.TrigAuto = LL_ADC_INJ_TRIG_FROM_GRP_REGULAR;
    LL_ADC_INJ_Init(ADC1, &ADC_INJ_InitStruct);

    LL_ADC_INJ_SetSequencerRanks(ADC1, LL_ADC_INJ_RANK_1, LL_ADC_CHANNEL_7);

    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_7, LL_ADC_SAMPLINGTIME_239CYCLES_5);

    LL_ADC_INJ_SetOffset(ADC1, LL_ADC_INJ_RANK_1, 0);

    /**Configure Injected Channel
    */
    LL_ADC_INJ_Init(ADC1, &ADC_INJ_InitStruct);

    LL_ADC_INJ_SetSequencerRanks(ADC1, LL_ADC_INJ_RANK_2, LL_ADC_CHANNEL_8);

    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_8, LL_ADC_SAMPLINGTIME_239CYCLES_5);

    LL_ADC_INJ_SetOffset(ADC1, LL_ADC_INJ_RANK_2, 0);

    /**Configure Injected Channel
    */
    LL_ADC_INJ_Init(ADC1, &ADC_INJ_InitStruct);

    LL_ADC_INJ_SetSequencerRanks(ADC1, LL_ADC_INJ_RANK_3, LL_ADC_CHANNEL_9);

    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_9, LL_ADC_SAMPLINGTIME_239CYCLES_5);

    LL_ADC_INJ_SetOffset(ADC1, LL_ADC_INJ_RANK_3, 0);

    /* Enable ADC */
    LL_ADC_Enable(ADC1);

    /* Wait for the ADC to stabilise, the plus one acts as ceil() */
    Time_Delay_Ms( (LL_ADC_DELAY_TEMPSENSOR_STAB_US / 1000) + 1 );

    /* Run ADC self calibration */
    LL_ADC_StartCalibration(ADC1);
    while (LL_ADC_IsCalibrationOnGoing(ADC1))
    {}

    /* Start the ADC conversions */
    LL_ADC_INJ_StartConversionSWStart(ADC1);
}

/**
  * @brief      Read latest ADC mesurements
  *
  * @param      i_out_raw  latest I_out raw value
  * @param      v_in_raw   latest V_in raw value
  * @param      v_out_raw  latest V_out raw value
  */
__INLINE void ADC_get_values(uint16_t *i_out_raw, uint16_t *v_in_raw, uint16_t *v_out_raw)
{
    *i_out_raw = ADC_i_out_raw;
    *v_in_raw  = ADC_v_in_raw;
    *v_out_raw = ADC_v_out_raw;
}

/**
 * @brief      This function handles ADC1 global interrupt.
 */
void ADC1_IRQHandler(void)
{
    LL_ADC_ClearFlag_JEOS(ADC1); // Clear the injected channel end of conversion flag

    ADC_i_out_raw = LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_1);
    ADC_v_in_raw  = LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_2);
    ADC_v_out_raw = LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_3);
}