#include <msp430g2231.h>

#define SCE  BIT0
#define SDIN BIT3
#define DC   BIT2
#define SCK  BIT4
#define RES  BIT1

unsigned int value;
unsigned int i;
volatile unsigned char bits;
unsigned short cnt;

void LCDSend(unsigned char,unsigned char);
void LCDClear(void);
//void bar(char Ones, char X, char Y);
void LCDInit(void);
void LCDXY(unsigned char X, unsigned char Y);
void LCDPrintC(char charsel);
