// ------------------------------------------
//  Author: Prof. Taeweon Suh
//          Computer Science & Engineering
//          College of Informatics, Korea Univ.
//  Date:   May 06, 2020
// ------------------------------------------

#include "csd_zynq_peripherals.h"
#include "uart_init.s"

#define	TIMER_INITIAL	0x2000000

.align 5

csd_vector_table:
	b .
	b .
	b .
	b .
	b .
	b .
	b csd_IRQ_ISR
	b .

.global main
main:

	// Disable interrupt: CPSR'I = 1
	cpsID i

	cps #0x12	   	       // IRQ mode
	ldr	r13,=irq_stack_top // Stack pointer setup for IRQ mode

	cps #0x13		          // supervisor mode
	ldr	r13,=svc_stack_top // Stack pointer setup for SVC mode

	cps #0x11		          // FIQ mode
	ldr	r13,=fiq_stack_top // Stack pointer setup for FIQ mode

	cps #0x1F	             // SYS mode

	// Set VBAR (Vector Base Address Register) to my vector table
	ldr     r0, =csd_vector_table
	mcr     p15, 0, r0, c12, c0, 0
	dsb
	isb

	// Enable interrupt: CPSR'I = 0
	cpsIE i

// ---------------------------
// Generic Interrupt Controller (GIC) setup - Begin
// ---------------------------

	// CPU Interface ID Register
	ldr r0, =GICC_IIDR
	ldr r3, [r0]

	// CPU Controller Type Register
	ldr r0, =GICD_TYPER
	ldr r3, [r0]

	// CPU Binary Pointer Register
	ldr r0, =GICC_BPR
	ldr r3, [r0]

	// Distributor Control Register
	ldr r0, =GICD_CTLR
	ldr r1, [r0]
	mov r2, #1       // Enable
	orr r1, r1, r2
	str r1, [r0]
	ldr r3, [r0]

	 // Interrupt Set-Enable Register 0
	ldr r0, =GICD_ISENABLER0
	ldr r1, [r0]
	mov r2, #1 << 29   // Enable #29 (Private Timer)
	orr r1, r1, r2
	str r1, [r0]
	ldr r3, [r0]

	// According to specifications,
	// Cortex-A9 supports 5-bit version of priority format [7:3] in secure world
	// ( 0 -> 8 -> 16 -> 24...)

	// Interrupt Priority Register #7
	ldr r0, =GICD_PRIOR7
	ldr r1, [r0]
	mov r2, #0x10 << 8    // Priority 16 for ID# 29 (Private Timer)
	orr r1, r1, r2
	str r1, [r0]
	ldr r3, [r0]

	// CPU Interface Control Register
	ldr r0, =GICC_CTLR
	ldr r1, [r0]
	mov r2, #1        // Enable
	orr r1, r1, r2
	str r1, [r0]
	ldr r3, [r0]

 	// CPU Interface Interrupt Priority Mask Register
	ldr r0, =GICC_PMR
	ldr r1, [r0]
	mov r2, #0xFF     // Lowest
	orr r1, r1, r2
	str r1, [r0]
	ldr r3, [r0]

// ---------------------------
// Generic Interrupt Controller (GIC) setup - End
// ---------------------------

	UART_init

// ---------------------------
// Private Timer setup - Begin
// ---------------------------

 	// Private Timer Load Register
	ldr r0, =PRIVATE_LOAD
 	ldr r1, =TIMER_INITIAL
 	str r1, [r0]

 	// Private Timer Control Register
  	ldr r0, =PRIVATE_CONTROL
 	mov r1, #9 << 8   // Prescalar
 	orr r1, r1, #7     // IRQ Enable, Auto-Reload, Timer Enable
 	str r1, [r0]

	ldr r2, =timer
	bl print_timer
// ----------------------------
// Private Timer setup - End
// ----------------------------

 	// Check out the counter value to make sure the counter is decrementing
	ldr r0, =PRIVATE_COUNTER
	ldr r1, [r0]
	ldr r2, [r0]
	ldr r3, [r0]
	ldr r4, [r0]
	ldr r5, [r0]
	ldr r6, [r0]
	ldr r7, [r0]
	ldr r8, [r0]

	ldr r0, =PRIVATE_COUNTER
	ldr r1, =PRIVATE_STATUS
	ldr r2, =GICD_ISPENDR0

forever:
	ldr r5, [r0]
	ldr r6, [r1]
	ldr r7, [r2]
	b forever


// ----------------------------
// Interrupt Service Routines (ISRs) - Begin
// ----------------------------

csd_IRQ_ISR:

	stmfd sp!, {r0-r12, lr}

 	// Interrupt Ack
  	ldr r0, =GICC_IAR
	ldr r3, [r0]

 	// increase and print timer
	stmfd sp!, {r3}
	bl increase
	ldr r2, =timer
	bl print_timer
	ldmfd sp!, {r3}

 	// Clear Interrupt Status bit
  	ldr r0, =PRIVATE_STATUS
  	mov r1, #1
	str r1, [r0]

 	// End-of-Interrupt
  	ldr r0, =GICC_EOIR
	str r3, [r0]

	ldmfd sp!, {r0-r12, lr}
	subs pc, lr, #4

 increase:
	ldr r0, =timer
	add r0, r0, #8	//초부터 업데이트

	//초의자리
	ldrb r1, [r0]
	add r1, r1, #1
	cmp r1, #58
	subhs r1, #10
	strb r1, [r0], #-1	//10초 단위로 넘어가기
	movne pc, lr
	//10초단위
	ldrb r1, [r0]
	add r1, r1, #1
	cmp r1, #54
	subhs r1, #6
	strb r1, [r0], #-2	//: 넘어가고 1분단위로 넘어가기
	movne pc, lr

	//분의자리
	ldrb r1, [r0]
	add r1, r1, #1
	cmp r1, #58
	subhs r1, #10
	strb r1, [r0], #-1	//10분 단위로 넘어가기
	movne pc, lr

	ldrb r1, [r0]
	add r1, r1, #1
	cmp r1, #54
	subhs r1, #6
	strb r1, [r0], #-2	//: 넘어가고 1시단위로 넘어가기
	movne pc, lr

	//시의 자리
	ldrb r1, [r0]
	add r1, r1, #1
	cmp r1, #58
	subhs r1, #10
	strb r1, [r0], #-1	//10시 단위로 넘어가기
	movne pc, lr

	ldrb r1, [r0]
	add r1, r1, #1
	cmp r1, #58
	subhs r1, #10
	strb r1, [r0], #-1	//: 넘어가고 끝(0x0d)으로 넘어가기

	mov pc, lr

 print_timer:
 	ldr r0, =uart_Channel_sts_reg0
 	ldr r1, [r0]
 	and r1, r1, #0x8
 	cmp r1, #0x8
 	bne print_timer

 	ldr r0, =uart_TX_RX_FIFO0
 	ldrb r3, [r2], #1	//문자 하나 읽고 다음 문자로 넘김
 	strb r3, [r0]		//출력

 	cmp r3, #0
 	moveq pc, lr

 	b print_timer
// ----------------------------
// Interrupt Service Routines (ISRs) - End
// ----------------------------

.data
.align 4

irq_stack:     .space 1024
irq_stack_top:
fiq_stack:     .space 1024
fiq_stack_top:
svc_stack:     .space 1024
svc_stack_top:

led_value: .word 	0xC3

 timer:
  	.byte 0x0D
 	.ascii "00"
 	.byte 0x3A
 	.ascii "00"
 	.byte 0x3A
 	.ascii "00"
 	.byte 0x00
