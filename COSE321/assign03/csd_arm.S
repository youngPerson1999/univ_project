// ------------------------------------------
//  Author: Prof. Taeweon Suh
//          Computer Science & Engineering
//          College of Informatics, Korea Univ.
//  Date:   March 29, 2020
// ------------------------------------------

#define csd_LED_ADDR 0x41200000
#define csd_SWI_ADDR 0x41210000

.extern csd_main

.global main
main:
	bl csd_main
	ldr r3, =csd_LED_ADDR
	mov r2, #0b00000001
	led_loop:
		mov r1, r0		//main의 duration(r0)을 r1에서 써준다
		str r2, [r3]	//led주소에 값을 넣어준다 -> led on
		dur_loop:
			subs r1, r1, #1
			blne dur_loop
		cmp r2, #0b10000000	//0번부터 7번까지 다켰는지 확인
		lsl r2, r2, #1		//shift left하면 led가 한개씩 켜짐
		blne led_loop
	b main
