#ifndef __time_H
#define __time_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void Time_Delay_Ms(uint32_t delay);
uint32_t Time_Elapsed_Ms(void);

#ifdef __cplusplus
}
#endif
#endif /* __time_H */