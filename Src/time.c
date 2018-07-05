#include "stm32f1xx.h"
#include "time.h"

static volatile uint32_t sysTick;

void Time_Delay_Ms(uint32_t delay)
{
    uint32_t current_time = Time_Elapsed_Ms();

    while ((Time_Elapsed_Ms() - current_time) < delay)
    {}
}

__INLINE uint32_t Time_Elapsed_Ms(void)
{
    return sysTick;
}

/**
 * @brief      This function handles the System tick timer.
 */
void SysTick_Handler(void)
{
    sysTick++;
}