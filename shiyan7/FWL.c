/*****************************************************************************
File name: HRL
Description: 流水灯实验
Author: 熊康
Version: V1.0
Date: 2022.3.27
*****************************************************************************/

#include<c8051f020.h>


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

void Close_WD(void){
	WDTCN=0xDE;
	WDTCN=0XAD;
}
void port_Init(void){

	XBR2=0X40;
	P74OUT=0X0C;
}
void delay_ms (int n){

	int i=0,j=0;
	for(i=0;i<n;i++)
	for(j=0;j<3000;j++)
	;
}
//流水灯函数
void Follow_Water_loop(int n){
	unsigned int p,q;
	  for(p=0;p<n;p++)
	  {
			P5=0XFE;
			delay_ms(30);
			for(q=0;q<9;q++)
			{
				P5=P5<<1;
				delay_ms(30);
			}
		}
}
//跑马灯函数
void Horse_Race_Loop(int n)
{
	unsigned int p,q;
	  for(p=0;p<n;p++)
	  {
			P5=0XFE;
			delay_ms(30);
			for(q=0;q<9;q++)
			{
				P5=P5<<1;
			P5|=0X01;
				delay_ms(30);
			}
		}
}
void mod1(void){//显示模式1：全亮并闪烁4次。
	int k;
	for(k=0;k<4;k++){
		P5=0x00;
		delay_ms(100);
		P5=0xff;
		delay_ms(100);
	}
}
void mod2(void){//显示模式2：点亮一个发光管,右循环。
  P5=0xfe;  
		delay_ms(100);  
	P5=0xfd;  
		delay_ms(100); 
	P5=0xfb;  
		delay_ms(100); 
	P5=0xf7; 
		delay_ms(100); 
	P5=0xef;  
		delay_ms(100); 
	P5=0xdf; 
		delay_ms(100); 
	P5=0xbf; 
		delay_ms(100); 
	P5=0x7f;  
		delay_ms(100);
}//右循环过程。

void mod3(void){//显示模式3：点亮一个发光管,左循环。
  P5=0x7f;
		delay_ms(100);  
	P5=0xbf;  
		delay_ms(100); 
	P5=0xdf;  
		delay_ms(100);  
	P5=0xef;  
		delay_ms(100); 
	P5=0xf7; 
		delay_ms(100); 
	P5=0xfb;  
		delay_ms(100); 
	P5=0xfd;  
		delay_ms(100);  
	P5=0xfe;  
		delay_ms(100);
}//左循环过程。
void  mod4(void){//显示模式4：单数P5与双数P5交替点亮4次。
	int k;
	for(k=0;k<4;k++){
		P5=0xaa;
		delay_ms(100);
		P5=0x55;
		delay_ms(100);	
	}
}

void PIO(void){
//配置交叉译码开关,使P0口成为/INT0
	P0MDOUT=0x00;//配置输出为漏极开路P0=0xff;
	XBR0=0x00;	
	XBR1=0x04;// /INT0使能XBR2=0x40;//交叉开关使能
} 

 void IT(void){
	EA=0;//CPU关中断，禁止一切中断
	EX0=1;//允许外部中断0中断5
	IT0=0;//配置为低电平触发
	PX0=1;//外部中断0为高优先级
	EA=1;//CPU开放中断
}

void main(){
//主函数
	WDTCN=0xde;
	WDTCN=0xad;//禁止看门狗
	port_Init();
	//Follow_Water_loop(5);
	SYSCLK_Init();
	//IT();
	PIO();
	P5=0xff;//初始状态P5为全部熄灭
	//while(1){
		//if(a==0)
			mod1();
		//else if(a==1)
			mod2();
		//else if(a==2)
			mod3();
		//else if(a==3)
			mod4();
	//}
	P5=0xff;
}
