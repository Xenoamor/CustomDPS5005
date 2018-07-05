/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  */

#include <stdio.h>
#include <errno.h>
#include <sys/unistd.h> // STDOUT_FILENO, STDERR_FILENO

#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_gpio.h"
#include "main.h"
#include "usart.h"

void USART_Init(void)
{
    LL_USART_InitTypeDef USART_InitStruct;

    LL_GPIO_InitTypeDef GPIO_InitStruct;

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1); // Peripheral clock enable

    // USART1 GPIO Configuration
    // PA9  ------> USART1_TX
    // PA10 ------> USART1_RX

    GPIO_InitStruct.Pin = USART1_TX_PC_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(USART1_TX_PC_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USART1_RX_PC_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
    LL_GPIO_Init(USART1_RX_PC_GPIO_Port, &GPIO_InitStruct);

    // USART1 interrupt Init
    //NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    //NVIC_EnableIRQ(USART1_IRQn);

    USART_InitStruct.BaudRate = 115200;
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(USART1, &USART_InitStruct);

    LL_USART_ConfigAsyncMode(USART1);

    setvbuf(stdout, NULL, _IONBF, 0); // Turn off stdout buffering. This ensure printf prints immediately

    LL_USART_Enable(USART1);
}

void USART_Transmit(uint8_t * data, uint32_t size)
{
    while (size)
    {
        while (!LL_USART_IsActiveFlag_TXE(USART1)) {} // Block until last byte is sent
        LL_USART_TransmitData8(USART1, *data);
        data++;
        size--;
    }
}

// Override _write function to enable printf use
int _write(int file, char * data, int len)
{
    if ((file != STDOUT_FILENO) && (file != STDERR_FILENO))
    {
        errno = EBADF;
        return -1;
    }

    USART_Transmit((uint8_t *)data, len);

    return len; // Number of bytes written
}