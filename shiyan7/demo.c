#include <C8051F020.h>
#include "time.h"

void SYSCLK_Init(void);				//系统时钟初始化
void WDT_Disable(void);				//禁用看门狗定时器
void PORT_Init(void);				//端口初始化
//void Delayms(unsigned int time);	//@C8051F020 24M
void MODE_1();
void MODE_2();
void MODE_3();
void MODE_4();


void main()
{
	WDT_Disable();
	PORT_Init();
	SYSCLK_Init();
	while(1){MODE_1();}
	//MODE_2();
	//MODE_3();
	//while(1){MODE_4()};
}


void Delayms(unsigned int time) //@C8051F020 24M 软件延时得出
{
	unsigned int i,j;
	for(i = 0; i < time; i++)
	{
		for(j = 0;j < 3390; j++);
	}	
}



void WDT_Disable(void)
{
	/* 禁用看门狗定时器 */
	WDTCN = 0xDE;
	WDTCN = 0xAD;
}


void SYSCLK_Init(void)
{
	/* 软件延时循环变量 */
	int i = 0;				
	
	/* 启动外部晶体振荡器 OSCXCN = 01100111B */
	/* [6:4] = 110B，晶体振荡；[2:0] = 111B，频率 > 6.7MHz */
	OSCXCN = 0x67;			  
	
	/* 等待晶体振荡器启动 SYSCLK = 2M 调试得延时约1ms */
	for(i = 0; i < 256; i++); 
	
	/* 等待振荡器稳定 等待OSCXCN.1 = 1*/
	while(!(OSCXCN & 0x80));
	
	/* OSCICN = 10001000B [7] = 1 使能时钟丢失检测器；[3] = 1 选择外部振荡器 */
	OSCICN = 0x88;
}
void PORT_Init(void)
{
	/* 使能交叉开关 */
	XBR2 = 0x40;
}

void MODE_1(){
	int i;
	for(i=0;i<6;i++){
		P5 = 0xF0;
		Delayms(100);
		P5 = 0x0F;
		Delayms(100);
	}
}


void MODE_2(){
	unsigned char temp = 0x5F;
	while(1){
		temp = temp>>1|temp<<7;
		P5 = temp;
		Delayms(500);
	}
}

void MODE_3(){
	unsigned char temp = 0x5F;
	while(1){
		temp = temp<<1|temp>>7;
		P5 = temp;
		Delayms(500);
	}
}
void MODE_4(){
	int i;
	for(i=0;i<4;i++){
		P5 = 0xAA;
		Delayms(500);
		P5 = 0x55;
		Delayms(500);
	}
}
