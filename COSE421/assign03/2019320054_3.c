#pragma GCC target ("thumb")

#include "sam.h"

int main()
{
	/* Initialize the SAM system */
	SystemInit();
	
	PORT->Group[0].PINCFG[8].reg = 0x2; // PA08: INEN = 1, PMUXEN = 0
	PORT->Group[0].PINCFG[9].reg = 0x2; // PA09: INEN = 1, PMUXEN = 0
	
	PORT->Group[0].PINCFG[6].reg = 0x0; // peripheral mux enable = 0
	PORT->Group[0].PINCFG[7].reg = 0x0; // peripheral mux enable = 0
	
	PORT->Group[0].DIR.reg = 0x3 << 6; // Direction: Output(6,7)
	
	while (1) {	
		PORT->Group[0].OUT.reg = (PORT->Group[0].IN.reg >> 8) << 6;
		// ... 9,8, 0000 0000 so  shift right 8 bits -> ... 9,8 -> shift left 6 -> ... 9,8,00 0000
	}
	return (0);
}
