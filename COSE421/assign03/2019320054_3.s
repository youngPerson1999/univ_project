/*
 * Assembler1.s
 *
 * Created: 2024-04-15 오전 12:48:54
 *  Author: muzis
 */ 
 
 .syntax unified
 .section .text
 .global main

 .thumb
 .thumb_func
 main:
	ldr r0, =0x41004446  // Pin Configuration Register: PA06
	ldrb r1, [r0]         // check out if it is 0
	ldr  r2, =0x0         // bit 0 = 0
	strb r2, [r0]         // 	 
	ldrb r3, [r0]         // check out if it is 0

	ldr r0, =0x41004447  // Pin Configuration Register: PA07
	ldrb r1, [r0]         // check out if it is 0
	ldr  r2, =0x0         // bit 0 = 0
	strb r2, [r0]         // 	 
	ldrb r3, [r0]         // check out if it is 0

	ldr r0, =0x41004448
	ldrb r1, [r0]
	ldr  r2, =0x2         //PA08: INEN = 1, PMUXEN = 0
	strb r2, [r0]         

	ldr r0, =0x41004449
	ldrb r1, [r0]
	ldr  r2, =0x2         //PA09: INEN = 1, PMUXEN = 0
	strb r2, [r0]         	 

	ldr r0, =0x41004400	//direction
	ldr r1, =0x000000C0	//bit  6,7 =1(out), 8,9 =0(in)
	str r1, [r0]

	ldr r0, =0x41004410	// Output Register
	ldr r1, =0x41004420	//Input Register

	switch_led:
		ldr r2, [r1]
		lsrs r2, #8
		movs r3, #0x3
		ands r3, r2	//r3: 11, 10, 01, 00 <- 9,8
		cmp r3, #0b11
		ldr r4, =0x000000C0  // bit 6,7 = 1
		beq turn_on
		cmp r3, #0b10
		ldr r4, =0x00000080
		beq turn_on
		cmp r3, #0b01
		ldr r4, =0x00000040
		beq turn_on
		ldr r4, =0x00000000
		turn_on:
			str r4, [r0]
		b switch_led