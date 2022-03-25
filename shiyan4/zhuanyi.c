/*****************************************************************************
File name: zhuanyi
Description: 数据转移
Author: 熊康
Version: V1.0
Date: 2022.3.25
*****************************************************************************/
#include "stdio.h"
#include "intrins.h"
#include "reg51.h"
unsigned char data d[10] _at_ 0x30; //申明变量d，片内数据存储区
unsigned char xdata x[10] _at_ 0x0200;  //申明变量d，片外数据存储区
unsigned char code *ptr0;       //申明指向程序存储区的指针变量
unsigned char xdata *ptr1;      //申明指向外部数据存储区的指针变量
unsigned char data *temp;
int i,j;
	
void main(){
	ptr0 = 0x0010;     //指向程序存储区地址为0x0010
	ptr1 = 0x0200;     //指向外部数据存储区地址为0x0200
	temp = d;
	
	for(i=0,j=0x20; i<10; i++,j++){
		x[i] = i;
	}
	
	//将程序存储器的数据搬运到数据存储区
	for(i=0; i<10; ptr0++,i++){
		d[i] = *ptr0; 
	}
	
	//将外部数据存储区的数据搬运到内部数据存储区
	for(i=0; i<10; ptr1++,i++){
		d[i] = *ptr1; 
	}
	
}