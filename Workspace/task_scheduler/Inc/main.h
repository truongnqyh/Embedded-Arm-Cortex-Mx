/*
 * main.h
 *
 *  Created on: Dec 15, 2023
 *      Author: ADMIN
 */

		/* Sequence of Stack frame 1(SF1) and Stack frame 2(SF2)
		 * -----> SF1
		 * xPSR
		 * Return address(PC)
		 * LR
		 * R12
		 * R3
		 * R2
		 * R1
		 * R0
		 * -----> SF2
		 * R11
		 * R10
		 * R9
		 * R8
		 * R7
		 * R6
		 * R5
		 * R4 */

#ifndef MAIN_H_
#define MAIN_H_

#define MAX_TASKS 				4U
#define INIT_XPSR				0x00100000U
/**********************           Systick clock         ************************/
#define SYSTICK_CLK 			1000U
#define SYSTICK_TIM_CLK 		16000000U
/**********************           Stack allocation         ************************/
#define SRAM_START              0x20000000U
#define SRAM_SIZE               ((128) * (1024))
#define SRAM_END 				((SRAM_START) + (SRAM_SIZE))
#define NUM_OF_KB_EACH_TASK 	1U
#define T1_STACK_START 			(SRAM_END)
#define T2_STACK_START 			(SRAM_END - (1 * (NUM_OF_KB_EACH_TASK * 1024)))
#define T3_STACK_START 			(SRAM_END - (2 * (NUM_OF_KB_EACH_TASK * 1024)))
#define T4_STACK_START 			(SRAM_END - (3 * (NUM_OF_KB_EACH_TASK * 1024)))
#define SCHED_STACK_START 		(SRAM_END - (4 * (NUM_OF_KB_EACH_TASK * 1024)))

#endif /* MAIN_H_ */
