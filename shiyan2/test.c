/*****************************************************************************
File name: Test2
Description: 学习观察内部RAM和调试程序
Author: 熊康
Version: V1.0
Date: 2022.3.18
*****************************************************************************/
#include "stdio.h"
#include "intrins.h"
#include "reg51.h"

unsigned char ww0 _at_ 0x30;
unsigned char ww1 _at_ 0x31;
unsigned char ww2 _at_ 0x40;
unsigned char ww3 _at_ 0x41;

unsigned char ee0 _at_ 0x50;
unsigned char ee1 _at_ 0x51;
unsigned char ee2 _at_ 0x52;    //变量写入各自的单元中

int k,x;

void main()
{
	//给两个两位数赋值
	ww0=0x08;
	ww1=0x09;
	ww2=0x08;
	ww3=0x02;    
	
	//求出总和
	k=10*ww0+ww1+10*ww2+ww3;   
	TI=1;
	printf("k=%d",k);
	TI=0;
	
	//分别求出百位十位个位的值
	ee0=k/100;
	x=k%100;
	ee1=x/10;
	ee2=x%10;    
}
