/*****************************************************************************
File name: shuju
Description: 数据采集系统设计
Author: 熊康
Version: V1.0
Date: 2022.4.8
*****************************************************************************/
#include <C8051F020.H>
#include "ZLG7289.h"


sfr16 DP = 0x82;//数据指针
sfr16 TMR3RL=0X92;//定时器3重装值
sfr16 TMR3=0X94 ;//定时器3计数器
sfr16 ADC0=0XBE ;//ADco数据
sfr16 ADC0GT =0XC4 ;//ADC0大于窗口
sfr16 ADC0LT =0XC6 ;//ADCo小于窗口
#define SYSCLK 24000000//系统时钟频率
#define SAMPLERATE0 50000//lADCO采样频率
#define INT_DEC 256//均值滤波计数值
void Sysclk_Init (void) ;
void Port_Init (void) ;
void ADC0_Init (void) ;
void Timer3_Init (int counts) ;
void ADC0_ISR (void);
long result;//数字滤波后的结果
	
void delay (int n) {	
	int i;
	while (n--){
		for (i = 0; i <3000; i++);
	}
}

void main (void){
	long temprature ;
	//精度为百分之一的温度值
	int temp_int, temp_frac;
	//温度的整数部分和小数部分
	WDTCN = 0xde ;
	WDTCN = 0xad ;
	
	Sysclk_Init( ) ;
	Port_Init();
	Timer3_Init (SYSCLK/SAMPLERATE0);
	ADC0_Init () ;
	ZLG7289_Init (35);
	ZLG7289_Reset () ;
	AD0EN = 1 ;
	EA = 1;
	while (1){
	EA= 0;
	temprature = result;
	EA = 1;
	// temprature = temprature - 41758;
	// temprature = (temprature *100L)/ 154 ;//分离整数与小数
	temp_int = temprature /100 ;
	temp_frac = temprature - (temp_int * 100);//显示温度值
	ZLG7289_Download (0, 0, 0, temp_int%100/10) ;
	ZLG7289_Download (0, 1, 1, temp_int%10);
	ZLG7289_Download (0, 2,0, temp_frac/10) ;
	ZLG7289_Download (0, 3, 0, temp_frac%10);
	delay (50);
	}
}

void Sysclk_Init( ) {
	int i ;
	OSCXCN = 0x67;
	for (i = 0 ; i <256; i++);
	while ( ! (OSCXCN &0x80));
	OSCICN = 0x88 ;
}

void Port_Init () {
	XBR1 =0x00 ;
	XBR2= 0x40;
	P0MDOUT |= 0x01 ;//使能Txo推挽输出
}

void ADC0_Init() {
	ADC0CN = 0X05;
	//ADCO禁止;正常跟踪mode;定时器3溢出ADco转换开始
	//ADCO数据左对齐
	REF0CN |= 0X07;
	//允许温度传感器片内Vref和vref输出缓冲器0000_0111
	AMX0SL |= 0X0F;
	//选择温度传感器作为ADc多路模拟转换输出
	ADC0CF = (SYSCLK / 2500000 - 1)<<3;
	//ADc时钟
	ADC0CF |=0x01;
	// PGA增益=2
	EIE2 |=0x02;
		//允许ADC中断
}
void Timer3_Init (int counts) {
	TMR3CN = 0x02;//停止定时器3，清除TF3;使用系统时钟作为时基
	TMR3RL = -counts ;//初始化重装值
	TMR3 = 0XFFFF;//设置立即重装
	EIE2 &= ~0X01;//禁止定时器3中断
	              //l1111_1110
	TMR3CN |=0x04 ;//启动定时器3
}
void ADC0_ISR()interrupt 15{
	static unsigned int_dec =INT_DEC;//数字滤波计数值
	static long accumulator = 0L;
	AD0INT = 0 ;//清除adc转换结束标志
	accumulator += ADC0 ;//读adc值并加到运行总数中
	int_dec--;//更新滤波计数值
	if(int_dec == 0)
		{
			int_dec =INT_DEC;//重设计数器
			result =accumulator >>8; //求平均值
			accumulator = 0L;//复位accumulator
		}
}
