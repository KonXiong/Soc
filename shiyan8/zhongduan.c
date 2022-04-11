/*****************************************************************************
File name: zhongduan
Description: 外部中断实验
Author: 熊康
Version: V1.0
Date: 2022.4.8
*****************************************************************************/

#include<C8051F020.h>
#define LED P5//宏定义。
unsigned int mod=0;
a=0;

//延时函数
void delay(unsigned int n){
	unsigned i,j;
	for(i=0;i<n;i++)
	for(j=0;j<1000;j++);
}

//系统时钟
void SYSCLK(void) {
	int i;
	OSCXCN=0x67;
	for(i=0;i<256;i++);
	while(!(OSCXCN&0X80));
	OSCXCN=0X88;
}

/*显示模式1：全亮并闪烁。*/
void mod1(void){
	int k;
	for(k=0;k<1;k++){
		LED=0x00;
		delay(100);
		LED=0xff;
		delay(100);
		}
}

/*显示模式2：点亮一个发光管,右循环*/
void mod2(void){  
		LED=0xfe;  
	delay(100);  
	LED=0xfd;  
	delay(100); 
	 LED=0xfb;  
	delay(100); 
	 LED=0xf7; 
	 delay(100); 
	 LED=0xef;  
	delay(100); 
	 LED=0xdf; 
	 delay(100); 
	 LED=0xbf; 
	 delay(100); 
	 LED=0x7f;  
	delay(100);
}

/*显示模式3：点亮一个发光管,左循环*/
void mod3(void){  
	LED=0x7f;
		delay(100);  
	LED=0xbf;  
	delay(100); 
	 LED=0xdf;  
	delay(100);  
	LED=0xef;  
	delay(100); 
	 LED=0xf7; 
	 delay(100); 
	 LED=0xfb;  
	delay(100); 
	 LED=0xfd;  
	delay(100);  
	LED=0xfe;  
	delay(100);
}

/*显示模式4：单数LED与双数LED交替点亮*/
void  mod4(void){
	int k;
	for(k=0;k<1;k++){
		LED=0xaa;
		delay(100);
		LED=0x55;
		delay(100);	
	}
}

//配置交叉译码开关,使P0口成为/INT0
void PIO(void){
	P0MDOUT=0x00;//配置输出为漏极开路P0=0xff;
	XBR0=0x00;	
	XBR1=0x04;// /INT0使能XBR2=0x40;//交叉开关使能
} 

/*外部中断0(或1)触发方式的控制*/
 void IT(void){
	EA=0;//CPU关中断，禁止一切中断
	EX0=1;//允许外部中断0中断5
	IT0=0;//配置为低电平触发
	PX0=1;//外部中断0为高优先级
	EA=1;//CPU开放中断
}
 
/*主函数*/
void main(){
	WDTCN=0xde;
	WDTCN=0xad;//禁止看门狗
	SYSCLK();
	IT();
	PIO();
	LED=0xff;//初始状态LED为全部熄灭
	while(1){
		if(a==0)
		mod1();
		else if(a==1)
		mod2();
		else if(a==2)
		mod3();
		else if(a==3)
		mod4();
	}
}

//外部中断0
void INT0(void) interrupt 0{
	mod++;
	a=mod%4;
}
