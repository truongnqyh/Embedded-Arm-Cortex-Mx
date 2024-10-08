/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#define IRQ_NUMBER_USART2 38
int main(void)
{
	/* Configure interrupt for USART2 IRQ number 38, Addr: 0x000000D8 */
	/* Mannual pend the pending bit for USART2 */
	uint32_t *pISPR = (uint32_t *)(0xE000E200 + ((IRQ_NUMBER_USART2 / 32) * 0x04));
	*pISPR = (1 << (IRQ_NUMBER_USART2 % 32));

	/* Enable interrupt for USART2 via NVIC */
	uint32_t *pISER = (uint32_t *)(0xE000E100 + ((IRQ_NUMBER_USART2 / 32) * 0x04));
	*pISER = (1 << (IRQ_NUMBER_USART2 % 32));
    /* Loop forever */
	for(;;);
}

void USART2_IRQHandler(void)
{
	printf(" go to USART2 interurpt \n");
}
