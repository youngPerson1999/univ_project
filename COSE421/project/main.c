
#pragma GCC target ("thumb")

#include "sam.h"

void GCLK_setup();
void USART_setup();
void PORT_setup();
void TC3_setup();
void TC4_setup();
void RTC_setup();
void EIC_setup();
void I2C_setup();
void LIS2DH_I2C_write(unsigned char, unsigned char);
void LIS2DH_I2C_read_multiple_data(unsigned char);
void move_motor(int tc3, int tc4);
void print_byte(unsigned char);
void print_string(unsigned char *, unsigned int);
void println_string(unsigned char *, unsigned int);

unsigned char buffer[6];
unsigned int buffer_count= 0;
unsigned char return_msg[] = {""};

int main()
{
	unsigned char x_msg[] = {"x = "};
	unsigned char y_msg[] = {"y = "};
	unsigned char z_msg[] = {"z = "};
	
	unsigned char reg_data;
	
	int i, k;
	
	/* Initialize the SAM system */
	SystemInit();
	
	GCLK_setup();
	
	TC3_setup();
	
	TC4_setup();
	
	USART_setup();

	PORT_setup();
	
	EIC_setup();
	
	I2C_setup();

	// * 3-axis accelerometer configuration
	//   ODR = 0101 --> 100Hz, Normal Mode (LPen = 0, HR = 0), X,Y,Z enable
	LIS2DH_I2C_write(0x20, 0x57); // write 0x50 to 0x20 (=CTRL_REG1) register

	while (1) {
		// ----------------------------------------------
		// 10 usec pulse to Trigger input in Ultrasonic sensor
		// --------------------------------------------
		PORT->Group[0].OUT.reg &= ~(1 << 17);
		
		RTC_setup();
		
		while (RTC->MODE0.INTFLAG.bit.CMP0 != 1) ;

		RTC->MODE0.INTFLAG.bit.CMP0 = 1; // clear overflow interrupt flag
		PORT->Group[0].OUT.reg &= ~(0 << 17);

		while (RTC->MODE0.INTFLAG.bit.CMP0 != 1) ;

		RTC->MODE0.INTFLAG.bit.CMP0 = 1; // clear overflow interrupt flag
		PORT->Group[0].OUT.reg &= ~(1 << 17);
		// --------------------------------------------

		// Now use RTC to measure the pulse width of echo input.
		RTC->MODE0.CTRL.bit.ENABLE = 0; // Disable first
		RTC->MODE0.CTRL.bit.MATCHCLR = 0; // Now just count...
		RTC->MODE0.COUNT.reg = 0x0; // initialize the counter to 0
		RTC->MODE0.CTRL.bit.ENABLE = 1; // Enable
		
		LIS2DH_I2C_read_multiple_data(0x28);
		print_string(x_msg, sizeof(x_msg));
		print_byte(buffer[1]); // OUT_X_H
		print_byte(buffer[0]); // OUT_X_L
				
		print_string(y_msg, sizeof(y_msg));
		print_byte(buffer[3]); // OUT_Y_H
		print_byte(buffer[2]); // OUT_Y_L
				
		print_string(z_msg, sizeof(z_msg));
		print_byte(buffer[5]); // OUT_Z_H
		print_byte(buffer[4]); // OUT_Z_L
				
		println_string(return_msg, sizeof(return_msg));
		
		for (i= 0; i<200000; i++ ) ;  // idle roughly for 0.2 second
	};

	return (0);
}

void GCLK_setup() {
	
	// OSC8M
	//SYSCTRL->OSC8M.bit.ENABLE = 0; // Disable
	SYSCTRL->OSC8M.bit.PRESC = 0;  // prescaler to 1
	SYSCTRL->OSC8M.bit.ONDEMAND = 0;
	//SYSCTRL->OSC8M.bit.ENABLE = 1; // Enable

	//
	// Generic Clock Controller setup for RTC
	// * RTC ID: #4
	// * Generator #0 is feeding RTC
	// * Generator #0 is taking the clock source #6 (OSC8M: 8MHz clock input) as an input

	GCLK->GENCTRL.bit.ID = 0; // Generator #0
	GCLK->GENCTRL.bit.SRC = 6; // OSC8M
	GCLK->GENCTRL.bit.OE = 1 ;  // Output Enable: GCLK_IO
	GCLK->GENCTRL.bit.GENEN = 1; // Generator Enable

	GCLK->CLKCTRL.bit.ID = 0x1B; // ID #ID (TC2, TC3)
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for TC2, TC3
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to TC4, TC5

	GCLK->CLKCTRL.bit.ID = 0x1C; // ID #ID (TC4, TC5)
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for TC4, TC5
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to TC4, TC5
	
	GCLK->CLKCTRL.bit.ID = 4; // ID #4 (RTC)
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for RTC
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to RTC!
	
	GCLK->CLKCTRL.bit.ID = 5; // ID #5 (EIC)
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for RTC
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to RTC!

	// * SERCOM2: USART
	// * Generator #0 is feeding USART as well
	//
	GCLK->CLKCTRL.bit.ID = 0x16; // ID #0x16 (SERCOM2: USUART)
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for USART
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to USART!

}

void EIC_setup() {
	// Interrupt configuration for EXTINT[3] via PA19
	
	EIC->CONFIG[0].bit.FILTEN3 = 1 ;    // filter is enabled
	EIC->CONFIG[0].bit.SENSE3 = 0x3 ;   // Both-edges detection
	EIC->INTENSET.bit.EXTINT3 = 1 ;     // External Interrupt 3 is enabled
	
	EIC->CONFIG[0].bit.FILTEN5 = 1 ;    // filter is enabled
	EIC->CONFIG[0].bit.SENSE5 = 0x1 ;   // Rising-edges detection
	EIC->INTENSET.bit.EXTINT5 = 1 ;     // External Interrupt 5 is enabled for right
	
	EIC->CONFIG[0].bit.FILTEN6 = 1 ;    // filter is enabled
	EIC->CONFIG[0].bit.SENSE6 = 0x1 ;   // Rising-edges detection
	EIC->INTENSET.bit.EXTINT6 = 1 ;     // External Interrupt 6 is enabled for middle
	
	EIC->CONFIG[0].bit.FILTEN7 = 1 ;    // filter is enabled
	EIC->CONFIG[0].bit.SENSE7 = 0x1;	// Rising-edges detection
	EIC->INTENSET.bit.EXTINT7 = 1 ;     // External Interrupt 7 is enabled for left
	
	EIC->CTRL.bit.ENABLE = 1 ;          // EIC is enabled
	//
	// NVIC setup for EIC (ID #4)
	//

	NVIC->ISER[0] = 0x101 << 4 ;  // Interrupt Set Enable for EIC
	NVIC->IP[1] = 0x40 << 0 ; // priority for EIC: IP1[7:0] = 0x40 (=b0100_0000, 2-bit MSBs)
}

void PORT_setup() {	
	// PORT setup for PA10 and PA11 (USART): SERCOM2
	PORT->Group[0].PINCFG[10].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1
	PORT->Group[0].PINCFG[11].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1

	PORT->Group[0].PMUX[5].bit.PMUXE = 0x03; // peripheral function D selected
	PORT->Group[0].PMUX[5].bit.PMUXO = 0x03; // peripheral function D selected
	
	// PORT setup for PA17: Trigger in Ultrasonic Sensor
	PORT->Group[0].PINCFG[17].reg = 0x0;		// PMUXEN = 0
	PORT->Group[0].DIRSET.reg = 0x1 << 17;		// Direction: Output
	PORT->Group[0].OUT.reg = 0 << 17 ;          // Set the Trigger to 0

	// PORT setup for PA19 to take the echo input from Ultrasonic sensor
	PORT->Group[0].PINCFG[19].reg = 0x41;		// peripheral mux: DRVSTR=1, PMUXEN = 1
	PORT->Group[0].PMUX[9].bit.PMUXO = 0x0;		// peripheral function A (EIC) selected: EXTINT[3]
	
	//PORT setup for pa7: left,pa6: middle, pa5: right
	PORT->Group[0].PINCFG[6].reg = 0x41;
	PORT->Group[0].PMUX[3].bit.PMUXE = 0x0;
	PORT->Group[0].PINCFG[7].reg = 0x41;
	PORT->Group[0].PMUX[3].bit.PMUXO = 0x0;
	PORT->Group[0].PINCFG[5].reg = 0x41;
	PORT->Group[0].PMUX[2].bit.PMUXO = 0x0;
	
	PORT->Group[0].DIR.reg = 0x3203 << 8; // Direction: Output
	PORT->Group[0].OUT.reg = 0x1001 << 9;	//initialize: forward
	//tc3 right :15 9-1, 8-2, left :pb9 21-3,  20-4 tc4
}

void I2C_setup() {
	NVIC->IP[3] = 0xc0 ; 
	//
	// PORT setup for PA22 and PA23 (I2C)
	//

	PORT->Group[0].PINCFG[22].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1
	PORT->Group[0].PINCFG[23].reg = 0x41; // peripheral mux: DRVSTR=1, PMUXEN = 1

	PORT->Group[0].PMUX[11].bit.PMUXE = 0x02; // peripheral function C selected
	PORT->Group[0].PMUX[11].bit.PMUXO = 0x02; // peripheral function C selected

	// Power Manager
	PM->APBCMASK.bit.SERCOM3_ = 1 ; // Clock Enable for I2C
	
	//
	// * SERCOM3: I2C
	// * Generator #0 is feeding I2C

	GCLK->CLKCTRL.bit.ID = 0x17; // ID #17 (SERCOM3: I2C)
	GCLK->CLKCTRL.bit.GEN = 0; // Generator #0 selected for I2C
	GCLK->CLKCTRL.bit.CLKEN = 1; // Now, clock is supplied to I2C!
	
	// I2C Setup (Host mode)
	SERCOM3->I2CM.CTRLA.bit.SWRST = 1 ; // software reset
	SERCOM3->I2CM.CTRLA.bit.ENABLE = 0 ; // Disable
	//	SERCOM3->I2CM.CTRLA.bit.LOWTOUTEN = 1 ; // SCL Low Timeout Enable

	SERCOM3->I2CM.CTRLA.bit.MODE = 0x5 ; // Host mode
	//SERCOM3->I2CM.BAUD.bit.BAUD = 0x27 ; // 100KHz SCL (0x27 = d'39)
	SERCOM3->I2CM.BAUD.bit.BAUD = 0x20 ; // 100KHz SCL (0x20 = d'32)
	SERCOM3->I2CM.CTRLA.bit.ENABLE = 1 ; // Enable
	SERCOM3->I2CM.STATUS.bit.BUSSTATE = 1 ; // IDLE state
	

}

void TC3_setup() {
	//right wheel

	//
	// PORT setup for PA14 ( TC3's WO[0] )
	//
	PORT->Group[0].PINCFG[14].reg = 0x41;		// peripheral mux: DRVSTR=1, PMUXEN = 1
	PORT->Group[0].PMUX[7].bit.PMUXE = 0x04;	// peripheral function E selected

	//
	// PORT setup for PA15 ( TC3's WO[1] )
	//
	PORT->Group[0].PINCFG[15].reg = 0x41;		// peripheral mux: DRVSTR=1, PMUXEN = 1
	PORT->Group[0].PMUX[7].bit.PMUXO = 0x04;	// peripheral function E selected

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
	//left wheel
	//
	// PORT setup for PB08 ( TC4's WO[0] )
	//
	PORT->Group[1].PINCFG[8].reg = 0x41;		// peripheral mux: DRVSTR=1, PMUXEN = 1
	PORT->Group[1].PMUX[4].bit.PMUXE = 0x04;	// peripheral function E selected

	// PORT setup for PB09 ( TC4's WO[1] )
	PORT->Group[1].PINCFG[9].reg = 0x41;		// peripheral mux: DRVSTR=1, PMUXEN = 1
	PORT->Group[1].PMUX[4].bit.PMUXO = 0x04;	// peripheral function E selected

	// Power Manager
	PM->APBCMASK.bit.TC4_ = 1 ; // Clock Enable (APBC clock) for TC3

	// TC4 setup: 16-bit Mode
	TC4->COUNT16.CTRLA.bit.MODE = 0;  // Count16 mode
	TC4->COUNT16.CTRLA.bit.WAVEGEN = 3 ; // Match PWM (MPWM)
	TC4->COUNT16.CTRLA.bit.PRESCALER = 6; // Timer Counter clock 31,250 Hz = 8MHz / 256
	TC4->COUNT16.CC[0].reg = 1000;  // CC0 defines the period
	TC4->COUNT16.CC[1].reg = 200;  // CC1 match pulls down WO[1]
	TC4->COUNT16.CTRLA.bit.ENABLE = 1 ;
	
	TC4->COUNT16.CC[1].reg = 0;  // stop by generating no pulse(initialize)
}

void RTC_setup() {
	//
	// RTC setup: MODE0 (32-bit counter) with COMPARE 0
	//
	RTC->MODE0.CTRL.bit.ENABLE = 0; // Disable first
	RTC->MODE0.CTRL.bit.MODE = 0; // Mode 0
	RTC->MODE0.CTRL.bit.MATCHCLR = 1; // match clear
	
	// 8MHz RTC clock  --> 10 usec when 80 is counted
	RTC->MODE0.COMP->reg = 80; // compare register to set up 10usec interval
	RTC->MODE0.COUNT.reg = 0x0; // initialize the counter to 0
	RTC->MODE0.CTRL.bit.ENABLE = 1; // Enable
}

void LIS2DH_I2C_write(unsigned char reg_addr, unsigned char data) {
	
	//
	SERCOM3->I2CM.ADDR.bit.ADDR = 0x30 ; // LIS2DH address (0x18) + Write (0)
	while((SERCOM3->I2CM.INTFLAG.bit.MB != 1) || (SERCOM3->I2CM.STATUS.bit.RXNACK != 0));
	SERCOM3->I2CM.DATA.bit.DATA = reg_addr ; //
	while((SERCOM3->I2CM.INTFLAG.bit.MB != 1) || (SERCOM3->I2CM.STATUS.bit.RXNACK != 0));

	//
	SERCOM3->I2CM.DATA.bit.DATA = data ; //
	while((SERCOM3->I2CM.INTFLAG.bit.MB != 1) || (SERCOM3->I2CM.STATUS.bit.RXNACK != 0));
	
}

void LIS2DH_I2C_read_multiple_data(unsigned char reg_addr) {
	
	//
	SERCOM3->I2CM.ADDR.bit.ADDR = 0x30 ; // LIS2DH address (0x18) + Write (0)
	while((SERCOM3->I2CM.INTFLAG.bit.MB != 1) || (SERCOM3->I2CM.STATUS.bit.RXNACK != 0));

	SERCOM3->I2CM.DATA.bit.DATA = reg_addr | 0x80; // MSB in register address should be '1'
	while((SERCOM3->I2CM.INTFLAG.bit.MB != 1) || (SERCOM3->I2CM.STATUS.bit.RXNACK != 0));

	//
	SERCOM3->I2CM.ADDR.bit.ADDR = 0x31 ; // LIS2DH address (0x18) + Read (1)
	
	SERCOM3->I2CM.INTENSET.bit.SB = 1;	
	//turn on interrupt mode on Slave Bus (to use interrupt to read data  from I2C)
	buffer_count = 0; //size initialization
	while(buffer_count < 6)
	{
		print_string("", 0);
	}
	
	SERCOM3->I2CM.INTENCLR.bit.SB = 1;
	//clear the interrupt enable bit of Slave Bus
	
	SERCOM3->I2CM.CTRLB.bit.ACKACT = 1 ; // Send NACK
	SERCOM3->I2CM.CTRLB.bit.CMD = 3 ; // Execute ACK succeeded by STOP condition

}


void move_motor(int tc3, int tc4){
	int bit_17 = (PORT->Group[0].OUT.reg >> 17) & 1;
	int output;
	if(tc3 == tc4){
		output = 0x1001 << 9;	//move forward
	}
	else {
		if(tc3 > tc4){
			//turn left
			output = 0x1002 << 8;	
			// 21:0, 20:1, 9:1, 8:0
			//tc3 -> forward, tc4 -> backward
		}
		else {
			//turn right
			output = 0x2001 << 8;
			// 21:1, 20:0, 9:0, 8:1
			//tc3 -> backward, tc4 -> forward
		}	
	}
	PORT->Group[0].OUT.reg = output;
	TC3->COUNT16.CC[1].reg = tc3;
	TC4->COUNT16.CC[1].reg = tc4;
	PORT->Group[0].OUT.reg = (PORT->Group[0].OUT.reg & ~(1 << 17)) | (bit_17 << 17);
}

unsigned int RTC_count, count_start, count_end;
unsigned int Num_US_interrupts = 0;

void EIC_Handler(void)
{
	unsigned int echo_time_interval;
	unsigned int distance;		// dist donesm't need to be global
	
	if(EIC->INTFLAG.bit.EXTINT3){
		EIC->INTFLAG.bit.EXTINT3 = 1 ; // Clear the EXTINT3 interrupt flag
		Num_US_interrupts++;
	
		if(Num_US_interrupts == 1) {
			count_start = RTC->MODE0.COUNT.reg;
		}
		else if(Num_US_interrupts == 2) {
			count_end   = RTC->MODE0.COUNT.reg;
			RTC_count = count_end - count_start;
			echo_time_interval = RTC_count / 8 ; // echo interval in usec (8MHz clock input)
			distance = (echo_time_interval / 2) * 0.034 ; // distance in cm
			distance = distance > 980 ? 2 : distance;
		
			distance = distance % 100;
			Num_US_interrupts = 0 ;
		}
	}
	if(distance >= 20){
		//move fast
		if (EIC->INTFLAG.bit.EXTINT6) {
			move_motor(650, 650);
			EIC->INTFLAG.bit.EXTINT6 = 1; // Clear the EXTINT6 interrupt flag
		}
		if (EIC->INTFLAG.bit.EXTINT5) {
			move_motor(400, 650);	////turn right
			EIC->INTFLAG.bit.EXTINT5 = 1; // Clear the EXTINT5 interrupt flag
		}
		if (EIC->INTFLAG.bit.EXTINT7) {
			move_motor(650, 400); // Call the turn_left function
			EIC->INTFLAG.bit.EXTINT7 = 1; // Clear the EXTINT7 interrupt flag
		}
	}
	else {
		//if distance < 20, move slowly
		if (EIC->INTFLAG.bit.EXTINT6) {
			move_motor(500, 500);
			EIC->INTFLAG.bit.EXTINT6 = 1; // Clear the EXTINT5 interrupt flag
		}
		if (EIC->INTFLAG.bit.EXTINT5) {
			move_motor(450, 500);	//turn right
			EIC->INTFLAG.bit.EXTINT5 = 1; // Clear the EXTINT5 interrupt flag
		}
		if (EIC->INTFLAG.bit.EXTINT7) {
			move_motor(500, 450); //turn_left
			EIC->INTFLAG.bit.EXTINT7 = 1; // Clear the EXTINT7 interrupt flag
		}
	}

}

void SERCOM3_Handler(void){
	
	buffer[buffer_count] = SERCOM3->I2CM.DATA.reg;
	
	if (buffer_count < 6) {
		SERCOM3->I2CM.CTRLB.bit.ACKACT = 0; // Send ACK
		SERCOM3->I2CM.CTRLB.bit.CMD = 2; // Execute ACK
	}
	else {
		SERCOM3->I2CM.CTRLB.bit.ACKACT = 1; // Send NACK
		SERCOM3->I2CM.CTRLB.bit.CMD = 3; // Execute ACK succeeded by STOP condition
	}
	
	buffer_count++;
}

void USART_setup() {
	// Power Manager
	PM->APBCMASK.bit.SERCOM2_ = 1 ; // Clock Enable (APBC clock)
	
	// USART setup: SERCOM2
	//
	SERCOM2->USART.CTRLA.bit.MODE = 1 ; // Internal Clock
	SERCOM2->USART.CTRLA.bit.CMODE = 0 ; // Asynchronous UART
	SERCOM2->USART.CTRLA.bit.RXPO = 3 ; // PAD3
	SERCOM2->USART.CTRLA.bit.TXPO = 1 ; // PAD2
	SERCOM2->USART.CTRLB.bit.CHSIZE = 0 ; // 8-bit data
	SERCOM2->USART.CTRLA.bit.DORD = 1 ; // LSB first
	//SERCOM5->USART.CTRLA.bit.SAMPR = 1 ; //

	SERCOM2->USART.BAUD.reg = 0Xfb15 ; // 9,600 bps (baud rate) with 8MHz input clock
	SERCOM2->USART.CTRLB.bit.RXEN = 1 ;
	SERCOM2->USART.CTRLB.bit.TXEN = 1 ;

	SERCOM2->USART.CTRLA.bit.ENABLE = 1;
	
}

void print_byte(unsigned char data) {

	unsigned char conv_data;

	while (SERCOM2->USART.INTFLAG.bit.DRE != 1) ; // DRE (Data Register Empty) is set when DATA is empty

	conv_data = (data & 0xF0) >> 4 ; // take out the upper 4-bit
	if (conv_data > 9) conv_data += 55 ; // A, B, ... F
	else               conv_data += 48 ; // 0, 1, ... 9
	
	SERCOM2->USART.DATA.reg	= conv_data ; // transmit the first number (ASCII code)

	//
	while (SERCOM2->USART.INTFLAG.bit.DRE != 1) ;

	conv_data = (data & 0x0F) ;  // take out the lower 4-bit
	if (conv_data > 9) conv_data += 55 ; // A, B, ... F
	else               conv_data += 48 ; // 0, 1, ... 9
	
	SERCOM2->USART.DATA.reg	= conv_data ; // transmit the second number (ASCII code)
	
};

void println_string(unsigned char * msg, unsigned int msg_size) {
	if(msg_size <= 0) return;

	unsigned count;

	count = 0;
	
	while (1) {
		if (SERCOM2->USART.INTFLAG.bit.DRE == 1) {
			if (count == msg_size - 1) {
				SERCOM2->USART.DATA.reg	= 13; // Line Feed
				while (!SERCOM2->USART.INTFLAG.bit.DRE); // wait until data register is empty
				SERCOM2->USART.DATA.reg	= 10; // Carriage Return
				break;
				} else {
				SERCOM2->USART.DATA.reg	= msg[count];
				count++;
			}
		}
	};
}

void print_string(unsigned char * msg, unsigned int msg_size) {
	if(msg_size <= 0) return;

	unsigned count;

	count = 0;
	
	while (1) {
		if (SERCOM2->USART.INTFLAG.bit.DRE == 1) {
			if (count == msg_size - 1)  break;
			else {
				SERCOM2->USART.DATA.reg	= msg[count];
				count++;
			}
		}
	};
}