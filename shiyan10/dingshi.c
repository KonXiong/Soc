/*****************************************************************************
File name: dingshi
Description: 定时器
Author: 熊康
Version: V1.0
Date: 2022.4.8
*****************************************************************************/
#include "C8051F020.H"
#include "ZLG7289.h"


unsigned char Time [8]; //按位记录时间的数组
unsigned char Key = 0XFE; //按键值
unsigned char count = 50; //计数,20ms为一周期,50次为1s
unsigned char second = 0; //秒针
unsigned char minute = 0; //分针
unsigned char hour =0; //时针

/*将十位数时间转化为按个位保存,记录到数组中*/
void MakeTime (void);

//系统时钟初始化函数,选用外部时钟源24M晶振
void Sysclk_Init (void){
	int i =0;
	OSCXCN = 0x77;
	for(i= 0;i< 3000; i++);
	while ( (OSCXCN & 0x88) ==0);
	OSCXCN =0x08;
}

/*关闭看门狗*/
void WatchDog_Init (void) {
	EA=0;
	WDTCN =0xDE;
	WDTCN = 0xAD;
	EA=1;
}

void PortIO_Init (void){
	XBR2 =0x40;
	P3MDOUT = 0x00;
	EIE2 =0x20;
}

//定时器1初始化函数
void Timer1_Init (void){
	CKCON &= 0xef;//0b_1110_1111,TlM=0,选择系统时钟源12分频
	TMOD = 0x10; //T1M1T1MO = 01,16位计数器
	//想要做到1s 的计数，在两个寄存器中放置初值，正好让计数40000次之后，达到65523产生计数溢出，记录一次计数
	TH1 =(65535 - 3700)/256;//20ms,取高8位
	TL1 =(65535 - 3700)%256; //20ms,取低8位
	IE |=0x88;//允许T1中断
	IP |=0x08; //TF1为高级中断
	TCON |=0x40; //启动T1
}

//延时函数
void delay (unsigned int n){
	unsigned int i;
	while (n--){
		for (i=0; i<700; i++);
	}
}

void main (void) {
	char i =0;
	WatchDog_Init();
	Sysclk_Init();
	PortIO_Init();
	ZLG7289_Reset ();
	ZLG7289_Init (35);
	delay (50);
	ZLG7289_Reset ();
	Timer1_Init();
	while (1){
	MakeTime ();
		for (i=7;i >= 0; i--){
			ZLG7289_Download (0, i, 0,Time [i]);
			delay (5);
		}
	}
}

void MakeTime(){
	Time[0] =hour / 10;
	Time [1] =hour %10;
	Time [2]=0x0A;
	//显示间隔符
	Time [3] =minute / 10;
	Time [4] = minute %10;
	Time [5] =0x0A;
	//显示间隔符
	Time [6] = second / 10;
	Time [7] = second % 10;
}
void Timer1_ISR (void) interrupt 3{
	TH1 =(65535 - 3700)/256;
	TL1 = (65535 -3700)%256;
	//重新装载初值
	count --;
	if (count == 0){
	count=50;
	second ++;
		if(second > 59){
			minute++;
			second =0;
				if(minute > 59){
					minute = 0;
					hour++;
					if(hour >23)
						hour = 0;
				}
		}
	}
}

void INT7_ISR(void) interrupt 19{
	Key = ZLG7289_Key ();
	P3IF &= 0x7E;
	switch (Key){
	case 10:hour++; break ;
	case 11:hour--; break ;
	case 12:minute++; break ;
	case 13:minute--; break ;
	case 14:second++; break ;
	case 15:second--; break ;
	default: break ;
	}
}
