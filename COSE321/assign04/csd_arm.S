// ------------------------------------------
//  Author: Prof. Taeweon Suh
//          Computer Science & Engineering
//          College of Informatics, Korea Univ.
//  Date:   April 27, 2020
// ------------------------------------------

#include "uart_init.s"
#include "csd_macro.s"

.global main

main:

	UART_init     // UART Initialization
	CSD_macro		//매크로 함수 실행

	ldr r0, =Input_data			//input의 주소를 r0에 load
	ldr r1, =Output_data		//output의 주소를 r1에 load
	eor r2, r2, r2			//r2에 0을 할당, 반복문의 인자로 활용

	CSD_macro

	firstloop:
		CSD_macro

		eor r3, r3, r3		//r3에 0을 할당, 반복문의 인자로 활용
		mov r4, r0			//임시로 사용할 input의 주소를 r4에 넣어준다
		add r5, r4, #4		//r5에 r4 다음 숫자 주소 할당(r4 + 4)

		CSD_macro

		secondloop:
			CSD_macro

			ldr r6, [r4]	//r6에 r4의 주소에 있는 값 저장
			ldr r7, [r5]	//r7에 r5의 주소에 있는 값 저장
			cmp r6, r7		//둘을 비교 NVCV update(r6 - r7)

			CSD_macro

			strge r6, [r5]	//ge(greater or equal signed) 두 주소를 바꿔줌
			strge r7, [r4]
			add r5, r5, #4
			add r4, r4, #4
			add r3, #1		//반복횟수 증가
			cmp r3, #31		//NZCV update

			blne secondloop	//같지 않으면 다시 loop 반복
		CSD_macro

		add r2, #1			//반복횟수 증가
		cmp r2, #31			//NZCV update

		CSD_macro

		blne firstloop		//같지 않으면 다시 loop 증가
	eor r2, r2, r2			//정렬된 input을 output에 저장하기 위해 반복문의 인자로 활용

	CSD_macro

	save:
		ldr r3, [r0]		//r3에 r0(input)가 가르키는 값을 넣어줌(-16 - +15)
		str r3, [r1], #4	//r1에 r3의 값을 저장하고 r1의 주소를 +4해줌
		add r0, #4			//r0의 주소도 +4
		add r2, #1			//반복횟수 증가
		cmp r2, #32			//32번하면 output에 모두 값이 들어가니 NZCV 업데이트
		blne save			//32번 반복할 때까지 계속 실행해줌

	CSD_macro
	nop

.data
.align 4

Input_data:
 	.word 2, 0, -7, -1, 3, 8, -4, 10
 	.word -9, -16, 15, 13, 1, 4, -3, 14
 	.word -8, -10, -15, 6, -13, -5, 9, 12
 	.word -11, -14, -6, 11, 5, 7, -2, -12

Output_data:
	.word 0, 0, 0, 0, 0, 0, 0, 0
 	.word 0, 0, 0, 0, 0, 0, 0, 0
 	.word 0, 0, 0, 0, 0, 0, 0, 0
 	.word 0, 0, 0, 0, 0, 0, 0, 0
