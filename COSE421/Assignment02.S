
/*
 * Assembler1.s
 *
 * Created: 7/27/2023 10:27:01 AM
 *  Author: suhtw
 */ 

 .syntax unified
  
 .section .text
 .globl asm_test
 
 // Prof. Suh: 
 // if directive is removed (.thumb, .thumb_func),
 // exception occurs because blx instruction is branching to Arm code..
 // 
 // Note: Cortex-M0+ only supports thumb instructions

 .thumb
 .thumb_func

 asm_test:

  	ldr r1, =Input_data
	ldr r0, =Output_data
	//r1에 input, r0에 output 주소 load
 	movs r2, #0
	//r2에 0 할당 반복문의 인자 역할
	mov r4, r0
	//r4에 r0(output) 주소 복사
	copy_input:
		ldr r3, [r1]	//r1에 저장되어 있는 값 load
		str r3, [r4]	//r4에 저장된 주소에 r3의 값 저장
		adds r1, #4		//r1 = r1 + 4 다음 값을 읽기 위해 4b만큼 이동
		adds r4, #4		//r4 = r4 + 4 다음 저장할 주소를 받아오기 위해 덧셈
		adds r2, #1		//반복문 카운트++
		cmp r2, #32		//32와 같은지 비교
		bne copy_input	//같지  않다 == 복사완료 아니므로 다시 반복

	movs r2, #0			//반복문 인자 역할

	first_loop:
		movs r3, #0		//이중 반복문 인자 역할
		movs r1, r0		//output 주소를 r1에 넣어줌
		adds r4, r1, #4	//그 다음 값이 있는 주소를 r4에 넣어줌
		second_loop:	//이중 반복문
			ldr r5, [r1]	//r1의 값을 r5에 load
			ldr r6, [r4]	//r4의 값을 r6에 load
			cmp r5, r6		//r5, r6값 비교
			blt continue	//r5가 작다면 swap안하고 넘어감
			str r5, [r4]	//r6가 작으므로 swap
			str r6, [r1]	//서로 바꿔서 store
			continue:		//반복문 카운트을 위한 함수
				adds r1, #4	//r1에 다음 주소를 위해 +4
				adds r4, #4	//r4에 다음 주소를 위해 +4
				adds r3, #1	//second_loop 카운트 세기 위해 +1
				cmp r3, #31	//0,1 비교, 1,2 비교,,, 31,32비교 이므로 31까지만 반복하기 위해 cmp
				bne second_loop	//31이 아니라면 계속 second_loop 반복
		adds r2, #1		//first_loop 카운트 세기 위해 +1
		cmp r2, #31		//second_loop와 마찬가지로 31까지만 cmp
		bne first_loop	//끝나지 않았다면 계속 first_loop 반복

	forever:	//메인 함수 끝났음을 알려주기 위해 삽입
		nop		//아무 연산도 하지 않음
		b forever//무한반복
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