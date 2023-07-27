// ------------------------------------------
//  Author: Prof. Taeweon Suh
//          Computer Science & Engineering
//          College of Informatics, Korea Univ.
//  Date:   May 29, 2022
// ------------------------------------------

.global main
main:

fig2:
	 /* Disable MMU, if enabled */
	mrc   p15, 0, r0, c1, c0, 0      /* read CP15 register 1 */
	bic   r0, r0, #0x1         /* clear bit 0 */
	mcr   p15, 0, r0, c1, c0, 0      /* write value back */

	/* Invalidate caches and TLBs */
	mov   r0,#0            /* r0 = 0  */
	mcr   p15, 0, r0, c8, c7, 0      /* invalidate TLBs */
	mcr   p15, 0, r0, c7, c5, 0      /* invalidate icache */
	mcr   p15, 0, r0, c7, c5, 6      /* Invalidate branch predictor array */

	ldr r0, =-1024
	mov r1, #3072
	mov r2, #0x400000
	bl init_loop

	ldr   r0,=csd_MMUTable_fig2      /* Load MMU translation table base */
	orr   r0, r0, #0x5B         /* Outer-cacheable, WB */
	mcr   15, 0, r0, c2, c0, 0      /* TTBR0 */

   // Clear PD0 in TTBCR (Translation Table Base Control Register)
	mrc p15, 0, r2, c2, c0, 2
	bic r2, r2, #(1<<4)   // Set PD0 to 0 = HW-based page table walk
	mcr p15, 0, r2, c2, c0, 2

   /* Enable MMU */
   // Set M-bit in SCTLR (System Control Register)
	mrc   p15, 0, r0, c1, c0, 0
	bic r0, r0, #(1<<12)  // I-bit = I$ disable
	bic r0, r0, #(1<<2)   // C-bit = $ disable
	orr r0, r0, #(1<<0)   // M-bit = MMU enable
	mcr   p15, 0, r0, c1, c0, 0   /* Enable MMU */
	dsb                       /* dsb   allow the MMU to start up */
	isb                       /* isb   flush prefetch buffer */

	bl sum

fig3:
	/* Disable MMU, if enabled */
	mrc   p15, 0, r0, c1, c0, 0      /* read CP15 register 1 */
	bic   r0, r0, #0x1         /* clear bit 0 */
	mcr   p15, 0, r0, c1, c0, 0      /* write value back */

	/* Invalidate caches and TLBs */
	mov   r0,#0            /* r0 = 0  */
	mcr   p15, 0, r0, c8, c7, 0      /* invalidate TLBs */
	mcr   p15, 0, r0, c7, c5, 0      /* invalidate icache */
	mcr   p15, 0, r0, c7, c5, 6      /* Invalidate branch predictor array */

	ldr r0, =-1024
	mov r1, #3072
	mov r2, #0x400000
	bl init_loop

	ldr   r0,=csd_MMUTable_fig3      /* Load MMU translation table base */
	orr   r0, r0, #0x5B         /* Outer-cacheable, WB */
	mcr   15, 0, r0, c2, c0, 0      /* TTBR0 */

   // Clear PD0 in TTBCR (Translation Table Base Control Register)
	mrc p15, 0, r2, c2, c0, 2
	bic r2, r2, #(1<<4)   // Set PD0 to 0 = HW-based page table walk
	mcr p15, 0, r2, c2, c0, 2

   /* Enable MMU */
   // Set M-bit in SCTLR (System Control Register)
	mrc   p15, 0, r0, c1, c0, 0
	bic r0, r0, #(1<<12)  // I-bit = I$ disable
	bic r0, r0, #(1<<2)   // C-bit = $ disable
	orr r0, r0, #(1<<0)   // M-bit = MMU enable
	mcr   p15, 0, r0, c1, c0, 0   /* Enable MMU */
	dsb                       /* dsb   allow the MMU to start up */
	isb                       /* isb   flush prefetch buffer */

	bl sum

forever:
	nop
	b forever

init_loop:
	str r0, [r2], #4
	add r0, r0, #1
	cmp r0, #0
	addeq r0, r0, #1	//-1 ´ÙÀ½ÀÌ 1
	sub r1, r1, #1
	cmp r1, #0
	bne init_loop
	mov pc, lr

sum:
	mov r0, #0x200000
	mov r1, #3072
	mov r2, #0
loop:
	ldr r3, [r0], #4
	add r2, r2, r3
	sub r1, r1, #1
	cmp r1, #0
	bne loop
	mov pc, lr
