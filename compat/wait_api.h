#pragma once

#include "platform.h"

static inline void wait_us(unsigned t)
{
	int curr = SysTick->VAL, last = curr;
	int deadline = curr - (SystemCoreClock / 1000000) * t;
	for (;;) {
		curr = SysTick->VAL;
		if (curr > last) {
			deadline += SysTick->LOAD;
		}
		if (curr < deadline) {
			break;
		}
		last = curr;
	}
}

static inline void wait_ms(unsigned t)
{
	HAL_Delay(t);
}

static inline void wait(float s)
{
	wait_ms((unsigned)(1000*s));
}

