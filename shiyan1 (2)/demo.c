/*****************************************************************************
File name: add
Description: Test Keil
Author: Xiong Kang
Version: V1.0
Date: 2022.3.18
*****************************************************************************/
#include "reg51.h"
#include "stdio.h"


void delaylm(int x);

void main(){
	int i=0;
	while(1)
	{
		i=i+1;
		TI=1;
		printf("i=%d\n",i);
		TI=0;
		delaylm(100);
	}
}

void delaylm(int m)
{
	int x,y;
	for(y=0;y<=10000;y++)
		for(x=0;x<=m;x++);
}
