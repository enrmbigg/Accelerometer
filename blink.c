#include <msp430g2231.h>
#include "Acce.h"


void LCDPrintC(char charsel)     // Prints one character on screen
{
        unsigned char char_row, chardata;
        unsigned int charpos;
        static const char table [30] =
        {
            0x00, 0x00, 0x00, 0x00, 0x00 , // sp 0x20
            0x00, 0x36, 0x36, 0x00, 0x00 , // : 0x21
            0x7F, 0x09, 0x09, 0x09, 0x06 , // P 0x22
            0x7F, 0x09, 0x19, 0x29, 0x46 , // R 0x23
            0x07, 0x08, 0x70, 0x08, 0x07 , // Y 0x24
            0x7C, 0x7C, 0x7C, 0x7C, 0x7C   // Block 0x25
        };

        for(char_row=0;char_row < 5;char_row++)
        {

        charpos = (charsel-0x20)*5;
        chardata = table[(charpos+char_row)];
        LCDSend(chardata, 1);     // Writes character data
        }

}

void LCDXY(unsigned char X, unsigned char Y) //Sets X & Y Axis
  {
			LCDSend((0x80 | X), 0);//column
			//	delay();
			LCDSend((0x40 | Y), 0);	//row
  }

void LCDSend(unsigned char data, unsigned char cd)
{

		  bits=0;
		  cnt=8;
		  // assume clk is hi
		  // Enable display controller (active low).
		  P1OUT &= ~SCE;  //RESET SCE

		  // command or data
		  if(cd == 1) {
			P1OUT |= DC;  //set to send data
		  }
		  else {  // reset to send command
			P1OUT &= ~DC;
		  }

		  ///// SEND SPI /////
		  bits=0x80; // bits is mask to select bit to send. select bit msb first

		  //send data
		  while (0<cnt--)
		  {
			// put bit on line
			// cycle clock
			P1OUT &= ~SCK;
			if ((data & bits)>0) P1OUT |= SDIN; else P1OUT &= ~SDIN;
			//Delay(1);
			P1OUT |= SCK;
			//Delay(2);
			// SHIFT BIT MASK 1 right
			bits >>= 1;
		  }
}


void LCDClear(void)
{
		  int i;

		  LCDSend(0x0C, 0);
		  LCDSend(0x80, 0);
		  for (i = 0;i < 504;i++)  // number of rows
		  {
			  LCDSend(0x00, 1);
		  }
		  //delay();
}

void LCDInit(void)
{

		  int i;

		  // Reset LCD
		  P1OUT &= ~SCE;          // RESET SCE to enable
		  // toggle RES
		  P1OUT |= RES;           // Set RES

		  for(i=0;i<100;i++)
			i=i;
		  P1OUT &= ~RES;          // reset RES
		  for(i=0;i<100;i++)
			i=i;
		  P1OUT |= RES;           // Set RES

		  // Cycle Clock
		  P1OUT &= ~SCK;
		  P1OUT |= SCK;

		  // Disable display controller.
		  P1OUT |= SCE;           // bring high to disable

		  for(i=0;i<100;i++)
			i=i;

		  // Send sequence of command
		  LCDSend( 0x21, 0 );  // LCD Extended Commands.
		  LCDSend( 0xC8, 0 );  // Set LCD Vop (Contrast).
		  LCDSend( 0x04, 0 );  // Set Temp coefficent to 2.
		  LCDSend( 0x13, 0 );  // LCD bias mode 1:100.
		  LCDSend( 0x20, 0 );  // LCD Standard Commands, Horizontal addressing mode.
		  LCDSend(0x09, 0);
		  for(i=0;i<100;i++)
			 i=i;
		  LCDSend( 0x08, 0 );  // LCD blank
		  for(i=0;i<100;i++)
			   i=i;
		  LCDSend( 0x0C, 0 );  // LCD in inverse mode.

}
void bar(char value)
{
			int i = 0;
			for(i = 0;i<value;i++)
			{
				if(2 == 1)
				{
					LCDSend(14,2);
				}
				else
				{
					LCDSend(14,2);
				}
			 }

}
void barline(int rows,,char detail){
		int i = 0;
		int num=0;
		detail = 0x00;
		for(i = 0;i<rows;i++){
			if(num == 1){
				lcdcmd(detail,2);
			}
			else
			{
				lcdcmd(detail,2);
			}
		}
	}
int main(void)
{

			  WDTCTL = WDTPW + WDTHOLD;  // Stop WDT

			  P1DIR |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6); // Setup pins for LCD
			  DCOCTL = CALDCO_1MHZ;                     // Set DCO to 1 MHz (MCLK = SMCLK = 1 MHz)
			  BCSCTL1 = CALBC1_1MHZ;
			  BCSCTL3 |= LFXT1S1;
			  BCSCTL1 |= DIVA_2;
//			  TAR = 0;                                  // Clean Timer A counter
			  TACCTL0 = CCIE;                             // CCR0 interrupt enabled
			  TACCR0 = 2576;                              // this count corresponds to 1 sec
			  TACTL = TASSEL_1 + MC_1 + ID_0;
			  _BIS_SR(GIE);

			  //Set interrupts
			   LCDInit();
			   LCDClear();


while(1){

					//reading Z
					ADC10CTL0 = ADC10SHT_2 + ADC10ON;
			  		ADC10CTL1 = INCH_7 + ADC10DIV_0;
			  		ADC10CTL0 =  ADC10SHT_3 + ADC10ON;
			  		__delay_cycles(5);
			  		ADC10CTL0 |= ENC + ADC10SC;
			  		__delay_cycles(100);
			  		ADC10CTL0 &= ~ENC;
			  		ADC10CTL0 &= ~(ADC10ON);
			  		unsigned int readingZ  = ADC10MEM;

			  		readingZ -= 400;
			  		if(readingZ > 300)

			  		readingZ = 300;
			  		readingZ = readingZ / 3;
			  		readingZ = readingZ % 76;
			  		bar(readingZ);
			  		barline(76 - readingZ,0);
}
}




