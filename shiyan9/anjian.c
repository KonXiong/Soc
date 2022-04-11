/*****************************************************************************
File name: anjian
Description: 按键接口与数码管输出
Author: 熊康
Version: V1.0
Date: 2022.4.8
*****************************************************************************/
#include <C8051F020.H>
#include"ZLG7289.h"
#include"Digital.h"
#include<intrins.h>

unsigned char key;        //按键值
unsigned char datt=0;     //待显示数据
unsigned char seg_nf=0xFE;//闪烁数码管
char seg_n=0;             //当前选中数码管
char mode=0xFF;           //显示模式

void delay_ms (int n){

	int i=0,j=0;
	for(i=0;i<n;i++)
	for(j=0;j<3000;j++)
	;
}
void Port_Init();
void Sys_clk();
void WatchDog_Init();
void Reset();               //复位函数
void main(void)
	{
		unsigned char i=0;
		WatchDog_Init();
		Sys_clk();
		Port_Init();
		//初始化芯片
		ZLG7289_Init(40);
		delay_ms(30);
		//复位ZLG7289
		ZLG7289_Reset();
		delay_ms(30);
	
	while(1) 
		{
		if(key != 0xFF)
			{
				//键值在0-9之间，显示在数码管上，传给datt
			if(key >= 0 && key <= 9)
				datt=key;
			else
				{
				switch(key)
					{
					case 10: seg_n--; break; //数码管左移
					case 11: seg_n++; break; //数码管右移
					case 12: mode=1;  break; //切换到显示模式1
					case 13: mode=2;  break;
					case 14: mode=3;  break;
					case 15: Reset(); break; //复位
					default: break;	
					}
				}
		if(seg_n>7)seg_n=0;
		if(seg_n<0)seg_n=7;
		key=0xff;
			}
			/*++++如果显示模式已改变，且无按键按下循环执行如下代码块+++++++*/
			// key值由中断函数控制，当其改变时，跳出while 循环
			while(mode!=0xff&&key==0xff)
				{
		switch(mode){
			case 1:
				digital_mod_01();
			  delay_ms(500);
			case 2:
				digital_mod_user();
			  delay_ms(500);
		  case 3:
				digital_loop_move();
			  delay_ms(500);
			break;
			default:
			break;
		}
	}
				//当函数是从while循环跳出时，执行下面if语句，将mode清零
	if(mode!=0xff&&key!=0xff)
		mode=0xff;
	//控制数码管闪烁位，设为当前选中位
	for(i=0,seg_nf=0xfe;i<seg_n;i++)
	{
		seg_nf <<=1;
		seg_nf |=0x01;
	}
	ZLG7289_Reset();
	ZLG7289_Flash(seg_nf);
	ZLG7289_Download(1,seg_n,0,datt);
	delay_ms(50);
}
}
	//端口配置初始化函数
void Port_Init()
{
	XBR2=0x40;
	P3MDOUT=0X00; //设置P3端口为数字输入
	EIE2=0X20;    //打开外部中断IN7，其端口为P3^7
	EA=1;         //开总中断
}
//外部晶振初始化函数
void Sys_clk()
{
	int i;
	OSCXCN=0x67;
	for(i=0;i<3000;i++);
	while ((OSCXCN&0x80)==0);
	OSCICN=0x0c;
}

//关闭看门狗初始化函数
void WatchDog_Init()
{
	EA=0;
	WDTCN=0xde;
	WDTCN=0xad;
	EA=1;
}

//复位重置函数
void Reset()
{
	ZLG7289_Reset();
	seg_n=0;
	seg_nf=0xfe;
	datt=0;
}

//外部中断7服务函数
//芯片检测到按键按下，通过P3^7端口产生外部中断信号
void INT7_ISR(void)interrupt 19
{
	EA=0;       //关闭总中断
	key=ZLG7289_Key();//获取键值
	if(key>=0&&key<=9)
		datt=key;
	P3IF&=0x7f;     //清零中断标志位
	EA=1;           //打开总中断
}
