// ------------------------------------------
//  Author: Prof. Taeweon Suh
//          Computer Science & Engineering
//          College of Informatics, Korea Univ.
//  Date:   May 06, 2020
// ------------------------------------------

#include "csd_zynq_peripherals.h"
#define csd_SWI_ADDR 0x41210000
#define csd_LED_ADDR 0x41200000

.extern csd_main

.global main
main:

	// Read Cache Type Register (CTR)
	mrc p15, 0, r1, c0, c0, 1

	// Read Cache Level ID Register (CLIDR)
	mrc p15, 1, r2, c0, c0, 1

	ldr r3, =csd_SWI_ADDR
	ldr r4, [r3]
	and r4, r4, #0b1
	cmp r4, #0b0

	ldrne r5, =csd_LED_ADDR
	mov	r6, #0b0
	strne r6, [r5]
///*
	@------------------------
	@ Disable/Enable Caches (L2)
	@------------------------
	ldr r0, =L2_reg1_ctrl

	moveq r1, #0x0	//disable
	movne r1, #0x1	//enable
	str r1, [r0]
	@------------------------
	@ Disable/Enable Caches (IL1, DL1)
	@------------------------
	mrc		p15, 0, r0, c1, c0, 0	@ read control register (CP15 register1)

	biceq		r0, r0, #4096		    @ disable I bit (Instruction Cache)
	orrne		r0, r0, #(1<<12)	    @ Enable I bit (Instruction Cache)

	biceq		r0, r0, #4		        @ disable C bit (Data and Unified Caches)
	orrne		r0, r0, #(1<<2)         @ Enable C bit (Data and Unified Caches)

	mcr		p15, 0, r0, c1, c0, 0	@ write control register (CP15 register2)

	// read SCTLR (System Control Register) to r0
	mrc	p15, 0, r0, c1, c0, 0

	bleq csd_main
	b main

forever:
	nop
	b forever
