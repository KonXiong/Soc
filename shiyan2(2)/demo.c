#include <stdio.h>
#include <intrins.h>

unsigned char a[5] _at_ 0x50; 
 
unsigned char ww0 _at_ 0x30;
unsigned char ww1 _at_ 0x31;
unsigned char ww2 _at_ 0x32;
unsigned char ww3 _at_ 0x33;
unsigned char ww4 _at_ 0x34;
 
unsigned char ee0 _at_ 0x40;
unsigned char ee1 _at_ 0x41;
unsigned char ee2 _at_ 0x42;
 
unsigned char bdata BZW _at_ 0x20;  //标志位

sbit BB0 = BZW^0;   //标志位0
sbit BB1 = BZW^1;   //标志位1
sbit BB2 = BZW^2;   //标志位2
 
unsigned char  xx;yy;zz;ACC;B;TL0;TH0;
int i,k,y,z,x;
 
void main()
{

			ww0=0x95;   //存储器窗口    察看30h
			ww1=0xa6;                // 察看31h
			ww2=0xb7;                // 察看32h
			ww3=0x88;                // 察看33h
			ww4=0x08;                // 察看34h
 
			k=ww0+ww1+ww2+ww3+ ww4;  // 观察和堆栈窗口 察看  k=0x282
			ww4=k/4;                     // 察看34h   =0xa0 
 
			ee0=ww4/100;         //察看40h   =0x01
			xx=ww4%100;         //察看  xx  观察和堆栈窗口 察看  xx=0x3c
			ee1=xx/10;            //察看41h  =0x06
			ee2=xx%10;     //察看42h  =0x00
        if  (ee0>2)       
            BB0=1;     //察看20h  位0
        if  (ee1>=5)    
            BB1=1;     //察看20h  位1
        if  (ee2<9)   
            BB2=1;     //察看20h  位2
 
	for(i=0,k=0x55;i<5;i++,k++)
		{
        a[i]=k;    // 察看 存储器窗口 (50h---54h）
		}
    ACC=0xa0;    //专用寄存器窗口   察看 A
    B=0xbb;       //专用寄存器窗口   察看 B
	switch(ACC+1){
		case 0xa1:
			TL0=0x77;TH0=0x88;   //存储器窗口   察看8ah,8ch 
			break; 
		case 0xa2:
		  TL0=0x33;TH0=0x44;   //存储器窗口   察看8ah, 8ch
		break;
	  default:
			 TL0=0x11;  TH0=0x22;   //存储器窗口   察看8ah, 8ch
		break; 
     }
		while(1);
}   
