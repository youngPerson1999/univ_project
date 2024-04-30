
#pragma GCC target ("thumb")

#include "sam.h"

void print_enter();

void GCLK_setup();
void USART_setup();
void PORT_setup();
void TC3_setup();
void TC4_setup();
void print_manual();


int main()
{
	char rx_data;
	_Bool	rxc_flag;	
	
	/* Initialize the SAM system */
    SystemInit();
	
	GCLK_setup();
	
	USART_setup();

	PORT_setup();
	
	TC3_setup(); 

	TC4_setup();
 
	print_manual();
	while(1) {
		rxc_flag = SERCOM5->USART.INTFLAG.bit.RXC; // check out RXC (Receive Complete) flag
		if (rxc_flag == 1)	{
			rx_data	= SERCOM5->USART.DATA.reg; // Read the received data
			SERCOM5->USART.DATA.reg	= rx_data; // Write the received data (echo back to PC)
			int tc3_cc1, tc4_cc1;
			tc3_cc1 = TC3->COUNT16.CC[1].reg;
			tc4_cc1 = TC4->COUNT16.CC[1].reg;
			
			if (rx_data == '1') {	//forward
				PORT->Group[0].OUT.reg = 0x1 << 8;	//in 1,3 = 0, in 2,4 = 1
				TC3->COUNT16.CC[1].reg = 200;
				TC4->COUNT16.CC[1].reg = 200;
			}
			else if (rx_data == '2') {	//backward
				PORT->Group[0].OUT.reg = 0x1 << 9;	//in 1,3 = 1, in 2,4 = 0
				TC3->COUNT16.CC[1].reg = 200;
				TC4->COUNT16.CC[1].reg = 200;
			}
			else if (rx_data == '3') {	//increase speed
				TC3->COUNT16.CC[1].reg = TC3->COUNT16.CC[1].reg + 100;  // Increase speed
				TC4->COUNT16.CC[1].reg = TC4->COUNT16.CC[1].reg + 100;  // Increase speed
			}
			else if (rx_data == '4') {	//decrease speed
				if(tc3_cc1 > 0) TC3->COUNT16.CC[1].reg = TC3->COUNT16.CC[1].reg - 100;  // Decrease speed
				if(tc4_cc1 > 0) TC4->COUNT16.CC[1].reg = TC4->COUNT16.CC[1].reg - 100;  // Decrease speed
			}
			else if (rx_data == '5') {	//turn left
				TC3->COUNT16.CC[1].reg = TC3->COUNT16.CC[1].reg + 100;  // Increase speed
				if(tc4_cc1 > 0) TC4->COUNT16.CC[1].reg = TC4->COUNT16.CC[1].reg - 100;  // Decrease speed
			}
			else if (rx_data == '6') {	//turn right
				if(tc3_cc1 > 0) TC3->COUNT16.CC[1].reg = TC3->COUNT16.CC[1].reg - 100;  // Decrease speed
				TC4->COUNT16.CC[1].reg = TC4->COUNT16.CC[1].reg + 100;  // Increase speed
			}
			else if (rx_data == '7') {	//stop
				TC3->COUNT16.CC[1].reg = 0;  // stop by generating no pulse
				TC4->COUNT16.CC[1].reg = 0;  // stop by generating no pulse
			}
			print_enter();
			print_manual();
		}
	};
	return (0);
}



void GCLK_setup() {
	
	// OSC8M
	//SYSCTRL->OSC8M.bit.ENABLE = 0; // Disable
	SYSCTRL->OSC8M.bit.PRESC = 0;  // prescalar to 1
	SYSCTRL->OSC8M.bit.ONDEMAND = 0;
	//SYSCTRL->OSC8M.bit.ENABLE = 1; // Enable

	//
	// Generic Clock Controller setup for TC3
	// * TC3 ID: #0x1B 
	// * Generator #0 is feeding TC3
	// * Generator #0 is taking the clock source #6 (OSC8M: 8MHz clock input) as an input
	//
	GCLK->GENCTRL.bit.ID = 0; // Generator #0
	GCLK->GENCTRL.bit.SRC = 6; // OSC8M
	GCLK->GENCTRL.bit.OE = 1 ;  // Output Enable: GCLK_IO
	GCLK->GENCTRL.bit.GENEN = 1; // Generator Enable
	
	GCLK->CLKCTRL.bit.ID = 0x1B; // ID #ID (TCC2, TC3)
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for TCC2, TC3
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to TCC2, TC3
	
	GCLK->CLKCTRL.bit.ID = 0x1C; // ID #ID (TC4, TC5)
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for TC4, TC5
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to TC4, TC5
	

}

void PORT_setup() {
	//8��: IN 1,3		9��: IN 2,4
	PORT->Group[0].DIR.reg = 0x3 << 8; // Direction: Output
	PORT->Group[0].OUT.reg = 0x1 << 8;	//initialize: forward
}


void TC3_setup() {

	//
	// PORT setup for PA18 ( TC3's WO[0] )
	//
	PORT->Group[0].PINCFG[18].reg = 0x41;		// peripheral mux: DRVSTR=1, PMUXEN = 1
	PORT->Group[0].PMUX[9].bit.PMUXE = 0x04;	// peripheral function E selected

	//
	// PORT setup for PA19 ( TC3's WO[1] )
	//
	PORT->Group[0].PINCFG[19].reg = 0x41;		// peripheral mux: DRVSTR=1, PMUXEN = 1
	PORT->Group[0].PMUX[9].bit.PMUXO = 0x04;	// peripheral function E selected

	// Power Manager
	PM->APBCMASK.bit.TC3_ = 1 ; // Clock Enable (APBC clock) for TC3

	//
	// TC3 setup: 16-bit Mode
	//

	TC3->COUNT16.CTRLA.bit.MODE = 0;  // Count16 mode
	TC3->COUNT16.CTRLA.bit.WAVEGEN = 3 ; // Match PWM (MPWM)
	TC3->COUNT16.CTRLA.bit.PRESCALER = 6; // Timer Counter clock 31,250 Hz = 8MHz / 256
	TC3->COUNT16.CC[0].reg = 1000;  // CC0 defines the period
	TC3->COUNT16.CC[1].reg = 200;  // CC1 match pulls down WO[1]	
	TC3->COUNT16.CTRLA.bit.ENABLE = 1 ;
	
	TC3->COUNT16.CC[1].reg = 0;  // stop by generating no pulse(initialize)
}


void TC4_setup() {

	//
	// PORT setup for PA22 ( TC4's WO[0] )
	//
	PORT->Group[0].PINCFG[22].reg = 0x41;		// peripheral mux: DRVSTR=1, PMUXEN = 1
	PORT->Group[0].PMUX[11].bit.PMUXE = 0x04;	// peripheral function E selected

	//
	// PORT setup for PA23 ( TC4's WO[1] )
	//
	PORT->Group[0].PINCFG[23].reg = 0x41;		// peripheral mux: DRVSTR=1, PMUXEN = 1
	PORT->Group[0].PMUX[11].bit.PMUXO = 0x04;	// peripheral function E selected

	// Power Manager
	PM->APBCMASK.bit.TC4_ = 1 ; // Clock Enable (APBC clock) for TC3

	//
	// TC4 setup: 16-bit Mode
	//

	TC4->COUNT16.CTRLA.bit.MODE = 0;  // Count16 mode
	TC4->COUNT16.CTRLA.bit.WAVEGEN = 3 ; // Match PWM (MPWM)
	TC4->COUNT16.CTRLA.bit.PRESCALER = 6; // Timer Counter clock 31,250 Hz = 8MHz / 256
	TC4->COUNT16.CC[0].reg = 1000;  // CC0 defines the period
	TC4->COUNT16.CC[1].reg = 200;  // CC1 match pulls down WO[1]
	TC4->COUNT16.CTRLA.bit.ENABLE = 1 ;
	
	TC4->COUNT16.CC[1].reg = 0;  // stop by generating no pulse(initialize)
}



void USART_setup() {
	
	//
	// PORT setup for PB22 and PB23 (USART)
	//
	PORT->Group[1].PINCFG[22].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1
	PORT->Group[1].PINCFG[23].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1

	PORT->Group[1].PMUX[11].bit.PMUXE = 0x03; // peripheral function D selected
	PORT->Group[1].PMUX[11].bit.PMUXO = 0x03; // peripheral function D selected

	// Power Manager
	PM->APBCMASK.bit.SERCOM5_ = 1 ; // Clock Enable (APBC clock) for USART
	
	//
	// * SERCOM5: USART
	// * Generator #0 is feeding USART as well
	//
	GCLK->CLKCTRL.bit.ID = 0x19; // ID #0x19 (SERCOM5: USART): GCLK_SERCOM3_CORE
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for USART
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to USART!

	GCLK->CLKCTRL.bit.ID = 0x13; // ID #0x13 (SERCOM5: USART): GCLK_SERCOM_SLOW
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for USART
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to USART!
	
	//
	// USART setup
	//
	SERCOM5->USART.CTRLA.bit.MODE = 1 ; // Internal Clock
	SERCOM5->USART.CTRLA.bit.CMODE = 0 ; // Asynchronous UART
	SERCOM5->USART.CTRLA.bit.RXPO = 3 ; // PAD3
	SERCOM5->USART.CTRLA.bit.TXPO = 1 ; // PAD2
	SERCOM5->USART.CTRLB.bit.CHSIZE = 0 ; // 8-bit data
	SERCOM5->USART.CTRLA.bit.DORD = 1 ; // LSB first

	SERCOM5->USART.BAUD.reg = 0Xc504 ; // 115,200 bps (baud rate) with 8MHz input clock

	SERCOM5->USART.CTRLB.bit.RXEN = 1 ;
	SERCOM5->USART.CTRLB.bit.TXEN = 1 ;

	SERCOM5->USART.CTRLA.bit.ENABLE = 1;
	
}

void print_enter() {
	while (SERCOM5->USART.INTFLAG.bit.DRE != 1) ; // DRE (Data Register Empty) is set when DATA is empty
	SERCOM5->USART.DATA.reg	= 10 ; // transmit LF (Line Feed)
	while (SERCOM5->USART.INTFLAG.bit.DRE != 1) ;
	SERCOM5->USART.DATA.reg	= 13 ; // transmit CR (Carriage Return)
};

void print_manual(){
	char *manual = "----------------- Vehicle Control ----------------\n\r1. Forward 2. Backward\n\r3. Speed Up 4. Speed Down\n\r5. Left turn 6. Right turn 7. Stop\n\r-------------------------------------------------\n\rSelect:  ";
	int i = 0;
	unsigned char conv_data;

	while(manual[i] != '\0')  {
		while (SERCOM5->USART.INTFLAG.bit.DRE != 1) ; // DRE (Data Register Empty) is set when DATA is empty
		conv_data = (unsigned char) (manual[i] & 0x000000FF)  ; //
		i++;
		SERCOM5->USART.DATA.reg	= conv_data ; // transmit the first number (ASCII code)
		//\n->line feed \r->carriage return
	}
}
