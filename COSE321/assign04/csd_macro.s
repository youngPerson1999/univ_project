#include "uart_regs.h"

.macro CSD_macro
	//r13 -> sp 그전까지 백업
	stmfd sp!, {r0-r12}	//현재 스택에 r0-r12저장 <- 나중에 복원
	add r0, sp, #52		//52더하는 이유: 레지스터 13개 -> 4바이트씩이니까 4*13
	mov r1, lr			//r0에 lr레지스터 저장 <- 나중에 복원
	sub r2, pc, #24		//pc를  2에 저장<- 나중에 복원, 24빼주는 이유: PC+8에 4instruction전이라 +16
	mrs r3, cpsr		//현재 cpsr을 r3에 저장<- 나중에 복원


	ldr	r12, =uart_Channel_sts_reg0
	ldr  r11, =uart_TX_RX_FIFO0	//r4에 rx주소 읽음

	ldr	r4, =12f	//밑줄 출력
	bl 1f
	//reguster back-up
	ldr r9, =10f		//레지스터 백업
	mov r8, #0			//몇번째인지 저장
	bl 3f

	//nzcv 출력
	ldr r4, =14f
	bl 1f
	//n
	mov r4, #0x80000000
	and r9, r3, r4
	cmp r9, r4
	ldreq r5, =0x4E
	ldrne r5, =0x6E
	bl 2f
	//z
	mov r4, #0x40000000
	and r9, r3, r4
	cmp r9, r4
	ldreq r5, =0x5A
	ldrne r5, =0x7A
	bl 2f
	//c
	mov r4, #0x20000000
	and r9, r3, r4
	cmp r9, r4
	ldreq r5, =0x43
	ldrne r5, =0x63
	bl 2f
	//v
	mov r4, #0x10000000
	and r9, r3, r4
	cmp r9, r4
	ldreq r5, =0x56
	ldrne r5, =0x76
	bl 2f

	ldr r4, =13f
	bl 1f
	//i
	mov r4, #0x80
	and r9, r3, r4
	cmp r9, r4
	ldreq r5, =0x49
	ldrne r5, =0x69
	bl 2f
	//f
	mov r4, #0x40
	and r9, r3, r4
	cmp r9, r4
	ldreq r5, =0x46
	ldrne r5, =0x66
	bl 2f

	ldr r4, =13f
	bl 1f

	ldr r4, =0x100020
	and r9, r3, r4
	//ARM
	cmp r9, #0x0
	ldreq r4, =17f
	//Thumb2
	cmp r9, #0x20
	ldreq r4, =18f
	//Jazelle
	cmp r9, #0x100000
	ldreq r4, =19f
	//ThumbEE
	cmp r9, r4
	ldreq r4, =20f

	bl 1f

	ldr r4, =15f
	bl 1f

	//mode print
	mov r4, #0b11111
	//User
	cmp r4, #0b10000
	ldreq r4, =21f
	//FIQ
	cmp r4, #0b10001
	ldreq r4, =22f
	//IRQ
	cmp r4, #0b10010
	ldreq r4, =23f
	//SVC
	cmp r4, #0b10011
	ldreq r4, =24f
	//Abort
	cmp r4, #0b10111
	ldreq r4, =25f
	//Undefined
	cmp r4, #0b11011
	ldreq r4, =26f
	//System
	cmp r4, #0b11111
	ldreq r4, =27f
	bl 1f
	//cpsr
	ldr r4, =0xf0000000
	mov r6, #28
	mov r7, r3
	bl 5f

	ldr r5, =0x29
	bl 2f

	ldr r4, =11f
	bl 1f

	ldr	r4, =12f	//밑줄 출력
	bl 1f

	//cpsr 복구
	msr cpsr_all, r3

	ldr r14, =10f
	ldmia r14!, {r0-r12}
	ldr r14, [r14]

	b 6f


1:	//print 해주는 함수
	push {lr}	//현재 주소 저장
	ldrb r5, [r4], #1	//r4에서 1바이트 읽어오고 r4  + 1

	bl 2f

	cmp r5, #0x00
	bne 1b

	pop {lr}	//다시 이동
	moveq pc, lr	//현재 주소에 저장된 레지스터 주소 넣어줌

2:	//wait&&transmit
	ldr r10, [r12]
	and r10, r10, #0x8
	cmp r10, #0x8
	bne 2b

	strb r5, [r11]

	mov pc, lr
3: //back-up registers	<- stack에 저장해둔 레지스터 이용
	ldmfd sp!, {r7}		//스택에 넣어둔 레지스터 로드 (자동으로 r0->r12순서)
	str r7, [r9], #4	//r7에  r9값 저장하고 +4 <- 반복문돌면서 계속 저장할거니까
	push {r9}

	push {lr}
	bl 4f
	pop {lr}

	cmp r8, #13
	popne {r9}
	blt 3b
	//r13부터 출력 r13 = sp, r14 = lr, r15 = pc
	//r0, r1, r2에 저장했음
	//13
	mov r7, r0
	push {lr}
	bl 4f
	pop {lr}
	//14
	mov r7, r1
	push {lr}
	bl 4f
	pop {lr}
	//15
	mov r7, r2
	push {lr}
	bl 4f
	pop {lr}

	mov pc, lr


4:
	push {lr}
	ldr r5, =0x72	//'r'
	bl 2b

	mov r6, r8			//r6에는 1의 자리수 넣을거임
	cmp r8, #10			//10이상인지 아닌지 확인
	subge r6, r6, #10	//10보다 크면 10빼줌 <-1의 자리수

	ldrge r5, =0x31		//10이상이면 1출력
	blge 2b

	add r5, r6, #0x30
	bl 2b

	ldr r5, =0x20		//스페이스 출력
	bl 2b
	cmp r8, #10
	bllt 2b				//10보다 작으면 스페이스 두번
	//'= 0x' 출력
	ldr r4, =16f
	bl 1b
	//레지스터 출력 앞자리부터
	ldr r4, =0xf0000000
	mov r6, #28
	bl 5f


	cmp r8, #3
	cmpne r8, #7
	cmpne r8, #11
	cmpne r8, #15

	ldrne r4, =13f
	ldreq r4, =11f
	bl 1b

	add r8, r8, #1
	pop {lr}
	moveq pc, lr

5:
	and r5, r4, r7	//7이랑 겹치는 부분-> 첫번째부터....
	lsr r5, r5, r6

	cmp r5, #10		//10보다 큰지 확인 -> 알파벳으로 출력할지 결정
	addlt r5, r5, #0x30
	addge r5, r5, #0x57

	push {lr}
	bl 2b
	pop {lr}
	sub r6, r6, #4
	lsr r4, r4, #4

	//4개출력후 _ 출력
	cmp r6, #12
	ldr r5, =0x5f
	push {lr}
	bleq 2b
	pop {lr}

	cmp r6, #-4
	bne 5b

	mov pc, lr

	.ltorg

6:
	nop

.data
.align 4
10: 	//register back-up
	.space 56, 0x00

11:
	.byte 0x0D
	.byte 0x0A
	.byte 0x00
12:
	.ascii "-----------------------------------------------------------------------------------------------------"
	.byte 0x0D
	.byte 0x0A
	.byte 0x00
13:
	.ascii ", "
	.byte 0x00
14:
	.ascii "cpsr ="
	.byte 0x00
15:
	.ascii " mode, current mode = "
	.byte 0x00
16:
	.ascii "= 0x"
	.byte 0x00
17:
	.ascii "ARM"
	.byte 0x00
18:
	.ascii "Thumb2"
	.byte 0x00
19:
	.ascii "Jazelle"
	.byte 0x00
20:
	.ascii "ThumbEE"
	.byte 0x00
21:
	.ascii "User(=0x"
	.byte 0x00
22:
	.ascii "FIQ(=0x"
	.byte 0x00
23:
	.ascii "IRQ(=0x"
	.byte 0x00
24:
	.ascii "SVC(=0x"
	.byte 0x00
25:
	.ascii "Abort(=0x"
	.byte 0x00
26:
	.ascii "Undefined(=0x"
	.byte 0x00
27:
	.ascii "System(=0x"
	.byte 0x00
.text
.endm
