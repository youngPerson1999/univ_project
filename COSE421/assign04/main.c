
#pragma GCC target ("thumb")

#include "sam.h"

void GCLK_setup();
void PORT_setup();
void RTC_setup();

int main()
{
	_Bool	c;
	
	/* Initialize the SAM system */
	SystemInit();

	GCLK_setup();
	
	PORT_setup();
	
	RTC_setup();

	c = 1;

	while (1) {
		if (RTC->MODE0.INTFLAG.bit.CMP0) {
			PORT->Group[0].OUT.reg = c << 18; // l/0 [18]: Output register
			c = c ^ 1; // toggle
			RTC->MODE0.INTFLAG.bit.CMP0 = 1; // clear overflow interrupt flag
		}
	}
	
	return (1);
	
}



void GCLK_setup() {
	
	// OSC8M
	SYSCTRL->OSC8M.bit.PRESC = 0;  // prescalar to 1
	SYSCTRL->OSC8M.bit.ONDEMAND = 0;

	GCLK->GENCTRL.bit.ID = 0; // Generator #0
	GCLK->GENCTRL.bit.SRC = 6; // OSC8M
	GCLK->GENCTRL.bit.OE = 1 ;  // Output Enable: GCLK_0
	GCLK->GENCTRL.bit.GENEN = 1; // Generator Enable
	
	GCLK->CLKCTRL.bit.ID = 4; // ID #4 (RTC)
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for RTC
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to RTC!

}

void PORT_setup() {

	//
	// PORT setup for PA14 (GCLK_IO[0]) to check out clock output using logic analyzer
	//
	PORT->Group[0].PINCFG[14].reg = 0x41;		// peripheral mux: DRVSTR=1, PMUXEN = 1 100_0001
	PORT->Group[0].PMUX[7].bit.PMUXE = 0x07;	// peripheral function H selected
	
	//
	// PORT setup for PA18: Output to Logical Analyzer
	//
	PORT->Group[0].PINCFG[18].reg = 0x0; // peripheral mux enable = 0
	PORT->Group[0].DIR.reg = 0x1 << 18; // Direction: Output
	PORT->Group[0].OUT.reg = 0 << 18 ;

}

void RTC_setup() {

	//
	// RTC setup: MODE0 (32-bit counter) with COMPARE 0
	//

	RTC->MODE0.CTRL.bit.ENABLE = 0; // Disable first peripheral
	RTC->MODE0.CTRL.bit.MODE = 0; // Mode 0 32bit counter
	RTC->MODE0.CTRL.bit.MATCHCLR = 1; // match clear
	
	RTC->MODE0.COMP->reg = 0x1000; // compare register to set up the period(1000) <- f=1ms
	RTC->MODE0.COUNT.reg = 0x0; // initialize the counter to 0
	RTC->MODE0.CTRL.bit.ENABLE = 1; // Enable peripheral
}