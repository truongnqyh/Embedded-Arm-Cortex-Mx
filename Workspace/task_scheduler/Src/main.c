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
#include <main.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/****************************** PROTOTYPE *********************************/
void task1_handler(void);
void task2_handler(void);
void task3_handler(void);
void task4_handler(void);

void enable_processor_faults(void);
void init_systick_timer(uint32_t TICK_HZ);
void init_tasks_stack(void);
void update_the_next_task(void);
void save_psp_value(uint32_t current_psp_value);
uint32_t get_psp_value(void);
__attribute__((naked)) void init_scheduler_stack(uint32_t sched_top_of_stack);
__attribute__((naked)) void switch_sp_to_psp(void);
/**************************************************************************/

/****************************** GLOBAL VARIABLE***************************/
uint32_t psp_of_stack[MAX_TASKS] = {T1_STACK_START, T2_STACK_START, T3_STACK_START, T4_STACK_START};
uint32_t task_handlers[MAX_TASKS];
uint32_t current_task = 0;
/*************************************************************************/

int main(void)
{
	enable_processor_faults(); /* Enable faults exception */

	init_scheduler_stack(SCHED_STACK_START);

	task_handlers[0] = (uint32_t)task1_handler;
	task_handlers[1] = (uint32_t)task2_handler;
	task_handlers[2] = (uint32_t)task3_handler;
	task_handlers[3] = (uint32_t)task4_handler;

	init_tasks_stack();

	init_systick_timer(SYSTICK_CLK);

	switch_sp_to_psp();

	task1_handler();
    /* Loop forever */
	for(;;);
}

void task1_handler(void)
{
	while(1)
	{
		printf("This is task 1\n");
	}
}

void task2_handler(void)
{
	while(1)
	{
		printf("This is task 2\n");
	}
}

void task3_handler(void)
{
	while(1)
	{
		printf("This is task 3\n");
	}
}

void task4_handler(void)
{
	while(1)
	{
		printf("This is task 4\n");
	}
}

void save_psp_value(uint32_t current_psp_value)
{
	psp_of_stack[current_task] = current_psp_value;
}

void update_the_next_task(void)
{
	current_task++;
	current_task = current_task % MAX_TASKS;
}

__attribute__((naked)) SysTick_Handler(void)
{
	/* Save the context of current task */
	__asm volatile("MRS R0,PSP");				/* Save the current running task PSP's value */
	__asm volatile("STMDB R0!,{R4-R11}");		/* Using that PSP value store SF2(R4-R11) */
	__asm volatile("PUSH {LR}");
	__asm volatile("BL save_psp_value");		/* Save the value of PSP for running task */

	/* Retrieve the context of next task */
	__asm volatile("BL update_the_next_task");	/* Decide the next task run */
	__asm volatile("BL get_psp_value");		    /* Get the previous PSP value */
	__asm volatile("LDMIA R0!,{R4-R11}");		/* Using that PSP value retrieve SF2(R4-R11) */
	__asm volatile("MSR PSP,R0");				/* Update PSP for the next task and exit */
	__asm volatile("POP {LR}");
	__asm volatile("BX LR");
}

void enable_processor_faults(void)
{
	/* Get base address of SHCRS */
	uint32_t *pSHCRS = (uint32_t *)0xE000ED24U;

	*pSHCRS |= (uint32_t)(1 << 16); /* Enable MemManage fault */
	*pSHCRS |= (uint32_t)(1 << 17); /* Enable Bus fault */
	*pSHCRS |= (uint32_t)(1 << 18); /* Enable Usage fault */
}

void init_systick_timer(uint32_t TICK_HZ)
{
	uint32_t *pSYST_RVR = (uint32_t *)0xE000E014U;
	uint32_t *pSYST_CVR = (uint32_t *)0xE000E018U;
	uint32_t *pSYST_CSR = (uint32_t *)0xE000E010U;
	uint32_t count_value = 0;

	/* Calculate value to create exception for each 1ms, the parameter N for RVR should be "N-1" to get N cycles */
	count_value = (SYSTICK_TIM_CLK/TICK_HZ) - 1;
	/* Clear reload value for RVR register */
	*pSYST_RVR &= (uint32_t)0x0U;
	/* Program reload value for RVR register */
	*pSYST_RVR |= (uint32_t)count_value;
	/* Clear current value for CVR register */
	*pSYST_CVR &= (uint32_t)0x0U;
	/* Program Control and Status register */
	*pSYST_CSR |= (uint32_t)(1 << 2); /* Choose processor clock as clock source */
	*pSYST_CSR |= (uint32_t)(1 << 1); /* Enable systick exception request */
	*pSYST_CSR |= (uint32_t)(1 << 0); /* Enable systick counter */
}

__attribute__((naked)) void init_scheduler_stack(uint32_t sched_top_of_stack)
{
	__asm volatile("MSR MSP,%0": :"r"(sched_top_of_stack):);
	__asm volatile("BX LR");
}

void init_tasks_stack(void)
{
	uint32_t *pPSP;
	uint32_t index = 0;
	uint32_t index_1 = 0;

	for (index = 0; index < MAX_TASKS; index++)
	{
		pPSP = (uint32_t *)psp_of_stack[index];
		pPSP--; /* xPSR */
		*pPSP = INIT_XPSR; /* Set thumb state bit to 1 -> use Thumb instruction */

		pPSP--; /* PC */
		*pPSP |= task_handlers[index]; /* Initialize return address PC */

		pPSP--; /* LR */
		*pPSP |= 0xFFFFFFFD; /* Initialize LR to go into thread mode after exit handler, from PSP to PSP */

		for (index_1 = 0; index_1 < 13; index_1++)
		{
			/* Initialize remaining registers */
			pPSP--;
			*pPSP = 0;
		}

		psp_of_stack[index] = (uint32_t)pPSP;
	}
}

uint32_t get_psp_value(void)
{
	return psp_of_stack[current_task];
}

__attribute__((naked)) void switch_sp_to_psp(void)
{
	/* Get PSP value */
	__asm volatile("PUSH {LR}");
	__asm volatile("BL get_psp_value");
	__asm volatile("MSR PSP,R0");
	__asm volatile("POP {LR}");

	/* Switch from MSP to PSP */
	__asm volatile("MOV R0,#0x02");
	__asm volatile("MSR CONTROL,R0");
	__asm volatile("BX LR");
}

void UsageFault_Handler(void)
{
	printf("Usagefault_Handler\n");
	while(1);
}


void HardFault_Handler(void)
{
	printf("Hardfault_Handler\n");
	while(1);
}

void MemManage_Handler(void)
{
	printf("MemManage_Handler\n");
	while(1);
}

void BusFault_Handler(void)
{
	printf("BusFault_Handler\n");
	while(1);
}




















