#include<lpc214x.h>
//REGISTER SELECT
#define RS_ON (IO0SET = 1<<20)
#define RS_OFF (IO0CLR = 1<<20)
//ENABLE 
#define EN_ON (IO0SET = 1<<25);
#define EN_OFF (IO0CLR = 1<<25);

void LCD_Init();
void LCD_Reset();
void LCD_CmdWrite(unsigned char cmdByte);
void LCD_DataWrite(unsigned char dataByte);
void LCD_SendHigherNibble(unsigned char dataByte);
void LCD_SendCmdSignals();
void LCD_SendDataSignals();
void LCD_DisplayString(unsigned char *ptr);

void LCD_Init(){
	IO0DIR |= (1U << 20) | (1U<<25) | 0X000F0000; //P0.20-RS , P0.25-EN , P0.16-19 D4-D7
	delay_ms(100);

	LCD_Reset();
	
	LCD_CmdWrite(0x28);//4-bit interface
	LCD_CmdWrite(0x0E);// display - on , cursor - on , blinking cursor - on
	LCD_CmdWrite(0x01);//Clear LCD
}

void LCD_Reset(){
	LCD_SendHigherNibble(0x30);
	LCD_SendCmdSignals();
	delay_ms(100);
	LCD_SendHigherNibble(0x30);
	LCD_SendCmdSignals();
	delay_ms(100);
	LCD_SendHigherNibble(0x30);
	LCD_SendCmdSignals();
	delay_ms(100);
	LCD_SendHigherNibble(0x30);
	LCD_SendCmdSignals();
	delay_ms(100);
}

void LCD_CmdWrite(unsigned char cmdByte){
	LCD_SendHigherNibble(cmdByte);
	LCD_SendCmdSignals();
	delay_ms(100);
	cmdByte = cmdByte << 4;
	LCD_SendHigherNibble(cmdByte);
	LCD_SendCmdSignals();
}

void LCD_DataWrite(unsigned char dataByte){
	LCD_SendHigherNibble(dataByte);
	LCD_SendDataSignals();
	delay_ms(100);
	dataByte = dataByte << 4;
	LCD_SendHigherNibble(dataByte);
	LCD_SendDataSignals();
}

void LCD_SendHigherNibble(unsigned char dataByte){
	IO0CLR = 0X000F0000;
	IO0SET = ((dataByte >> 4) & 0x0F) << 16;
}

void LCD_SendCmdSignals(){
	RS_OFF;
	EN_ON;
	delay_ns(500);
	EN_OFF;
}

void LCD_SendDataSignals(){
	RS_ON;
	EN_ON;
	delay_ns(450);
	EN_OFF;
}
