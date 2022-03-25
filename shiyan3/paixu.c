/*****************************************************************************
File name: paixu
Description: 数据排序
Author: 熊康
Version: V1.0
Date: 2022.3.25
*****************************************************************************/
#include "stdio.h"
#include "intrins.h"
#include "reg51.h"
unsigned char data sum1 _at_ 0x40;
unsigned char data sum2 _at_ 0x41;
unsigned char data ave _at_ 0x50;
unsigned char data a[10] _at_ 0x30; 
int i,j,temp;




void main(){
	
//数组赋值
for(i=0,j=0x0C;i<10;i++,j--){
        a[i]=j;    // 数组从55到45
}
	
//冒泡排序
for(i=0; i<10; i++){
    for(j=9; j>i; j--){
        if(a[j]<a[j-1]){
            temp = a[j];
            a[j] = a[j-1];
            a[j-1] = temp;
        }
    }
}

//计算和和平均值
temp = 0;
for(i=1; i<9; i++){
    temp = temp + a[i];
}
sum1 = temp / 10;  //和的十位
sum2 = temp % 10;  //和的各位
ave = temp / 8;  //平均值
}
