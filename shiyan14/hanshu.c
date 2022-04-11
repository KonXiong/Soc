/*****************************************************************************
File name: hanshu
Description: 数据采集系统设计
Author: 熊康
Version: V1.0
Date: 2022.4.8
*****************************************************************************/
#include <C8051F020.H>
#include "ZLG7289.h"



sfr16 DP      = 0x89;      //数据指针
sfr16 TMR3RL  = 0x92;      //定时器3重装值
sfr16 TMR3    = 0X94;      //定时器3计数器
sfr16 ADC0    = 0XBE;      //ADC0数据
sfr16 ADC0GT  = 0XC4;      //ADC0大于窗口
sfr16 ADC0LT  = 0XC6;      //ADC0小于窗口
sfr16 DAC0    = 0xd2;

#define SYSCLK    24000000 //系统时钟频率
#define SAMPLERATE0  50000 //ADC0采样频率
#define INT_DEC    256     //均值滤波计数值
#define VREF 2430

void Sysclk_Init(void);
void Port_Init(void);
void ADC0_Init(void);
void Timer3_Init(int counts);
void ADC0_ISR(void);
void DAC0_Init(void);

long result;               //数字滤波后的结果

void delay(int n){
    int i;
    while(n--){
	for(i=0;i<3000;i++);
	}
}

void main(void){
    long voltage;
    int count;
    int UP = 0x10;
    
    WDTCN = 0xde;
    WDTCN = 0xad;
 
    Sysclk_Init();
    Port_Init();
    Timer3_Init(SYSCLK/SAMPLERATE0);
    ADC0_Init();
    DAC0_Init();
    ZLG7289_Init(35);
    ZLG7289_Reset();

    AD0EN=1;
    EA=1;
    while(1){
	/*方波*/
	if(count<=4096)
	{
		count+=UP;
	}
	else
	{
		count=0;
	}
    DAC0=count;
    delay(100);
    voltage =result*VREF/4096;
    ZLG7289_Download(0,0,1,voltage/1000);
    ZLG7289_Download(0,1,0,voltage%1000/100);
    ZLG7289_Download(0,2,0,voltage%100/10);
    ZLG7289_Download(0,3,0,voltage%10);
    delay(50);
    }
}

void Sysclk_Init(){
	int i;
	OSCXCN=0x67;
	for(i=0;i<256;i++);
	while(!(OSCXCN & 0x80));
	OSCICN=0x88;
}

void Port_Init(){
	XBR1=0X00;
	XBR2=0X40;
}

void ADC0_Init(){
	ADC0CN =0X44;                 //ADC禁止；正常跟踪mode；定时器3溢出ADC0转换开始
	                              //ADC数据左对齐
	REF0CN=0X07;                  //允许温度传感器片内Vref和Vref输出缓冲器
	AMX0SL=0x00;                  //AIN0作单独输入
	ADC0CF=(SYSCLK/2500000-1)<<3;
	ADC0CF |= 0x00;               //PGA增益=1
	EIE2 |=0X02;                  //允许ADC中断
}

void Timer3_Init(int counts){
	TMR3CN=0X02;                  //停止定时器3，清除TF3；使用系统时钟作为时基
	TMR3RL=-counts;               //初始化重装值
	TMR3=0XFFFF;                  //设置立即重装
	EIE2 &= ~0X01;                //禁止定时器3中断
	TMR3CN |=0X04;                //启动定时器3
}

void DAC0_Init(){
	DAC0CN=0x80;  //使能DAC0
	DAC0L=0x00;
	DAC0H=0x00;
}

void ADC0_ISR() interrupt 15
{
 	static unsigned int_dec=INT_DEC; //数字滤波计数值
	static long accumulator=0L;
	AD0INT=0;                        //清除adc转换结束标志
	accumulator+=ADC0;               //读adc值并加到运行总数中
	int_dec--;                       //更新滤波计数值
	if(int_dec==0){
		int_dec=INT_DEC;               //重设计数器
		result=accumulator>>8;         //求平均值
		accumulator=0L;                //复位accumulator
	}
}
