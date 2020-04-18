#include<lpc214x.h>
#include<stdio.h>
#define PLOCK 0X00000400

struct rtcT {
	unsigned char sec;
	unsigned char min;
	unsigned char hour;
	unsigned char dom;
	unsigned char month;
	unsigned int year;
};
void systemInit(void){
 PLL0CON = 0x01;
 PLL0CFG = 0x24;
 PLL0FEED = 0xAA;
 PLL0FEED = 0x55;
 while( !( PLL0STAT & PLOCK ))
 { ; }
 PLL0CON = 0x03;
 PLL0FEED = 0xAA; 
 PLL0FEED = 0x55;
 VPBDIV = 0x01; 
}

void uartInit(void){
 //configurations to use serial port
PINSEL0 |= 0x05; // P0.0 & P0.1 ARE CONFIGURED AS TXD0 & RXD0
U0LCR = 0x83; /* 8 bits, no Parity, 1 Stop bit */
U0DLM = 0; 
U0DLL = 8; // 115200 baud rate
U0LCR = 0x03; /* DLAB = 0 */
U0FCR = 0x07; /* Enable and reset TX and RX FIFO. */
}

void rtcInit(){
	CCR = (1<<4) | (1<<0);
}
void rtcSetDate(struct rtcT *rtc){
	SEC   = rtc->sec;
	MIN   = rtc->min;
	HOUR  = rtc->hour;
	DOM   = rtc->dom;
	MONTH = rtc->month;
	YEAR  = rtc->year;
}

void rtcGetDate(struct rtcT *rtc){
	rtc->sec   = SEC;
	rtc->min   = MIN;
	rtc->hour  = HOUR;
	rtc->dom   = DOM;
	rtc->month = MONTH;
	rtc->year  = YEAR;
}

void delay(int cnt){
	int i,j;
	for(i=0;i<cnt;i++)
		for(j=0;j<10000;j++);
}

void runDCMotor(int direction,int dutycycle){
 IO0DIR |= 1U << 28; //set P0.28 as output pin
 PINSEL0 |= 2 << 18; //select P0.9 as PWM6 (option 2)
 if (direction == 1)
IO0SET = 1 << 28; //set to 1, to choose anti-clockwise direction
else
 IO0CLR = 1 << 28; //set to 0, to choose clockwise direction

 PWMPCR = (1 << 14); // enable PWM6
 PWMMR0 = 1000; // set PULSE rate to value suitable for DC Motor operation
 PWMMR6 = (1000U*dutycycle)/100; // set PULSE period
 PWMTCR = 0x00000009; // bit D3 = 1 (enable PWM), bit D0=1 (start the timer)
 PWMLER = 0X70; // load the new values to PWMMR0 and PWMMR6 registers
}
int readLDR(){
	PINSEL1 |= 1 << 24;//AD0.1 - P0.28
	AD0CR = (1 << 1) | (1 << 24) | (1 << 21) ;
	while( (AD0GDR & (unsigned long) 1 << 31) == 0x00);
	return (( AD0GDR >> 6) & 0X3FF);
}
int readTemp(){
	PINSEL1 |= 1 << 26;//AD0.1 - P0.29
	AD0CR = (1 << 2) | (1 << 24) | (1 << 21) ;
	while( (AD0GDR & (unsigned long) 1 << 31) == 0x00);
	return (( AD0GDR >> 6) & 0X3FF);
}
void serialPrintStr(char *buf){
	short int i;
	char ch;
	for(i=0;(ch = buf[i])!='\0';i++){
				while((U0LSR & 1U<<5) == 0X00);
				U0THR = ch;
		}
	}
	
void serialPrintInt(int value){
	char buf[50];
	sprintf((char *)buf,"%d\x0d\x0a",value);
	serialPrintStr(buf);
}
int main(){
	unsigned char msg[50];
	struct rtcT rtc;
	systemInit();
	uartInit();
	
	rtcInit();
	
	//Set the initial value of time
	rtc.sec   = 00;
	rtc.min   = 00;
	rtc.hour  = 10;
	rtc.dom   = 14; 
	rtc.month = 04;
	rtc.year  = 2020;
	
	rtcSetDate(&rtc);
	
	while(1){
		int LDRValue;
		rtcGetDate(&rtc);
		sprintf((char *)msg,"Time:%2d:%2d:%2d \n Date:%2d/%2d/%2d \n",rtc.hour,rtc.min,rtc.sec,rtc.dom,rtc.month,rtc.year);
		serialPrintStr((char *) msg);
		LDRValue = readLDR();
		serialPrintInt(LDRValue);
		delay(10);
	}
}

	

	










