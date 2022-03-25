/***************************************************************************** 
File name: Test 
Description: 熟悉C语言编程 
Author: 熊康 
Version: V1.0 
Date: 2022.3.18 
*****************************************************************************/  
#include "reg51.h"  
#include "stdio.h"  
  
void main(){  
    int i=0;  
    int a=3,b=6;  
    int c=0;  
    float s=0;  
    while(1)  
    {  
        //two numbers add  
        c=a+b;    
        TI=1;   //Open Serial  
        printf("c=%d\n",c);  //print to the Serial  
        TI=0;  //Close Seiral  
        P1=i;  //Open Port 1  
  
        //Conpare get the smaller  
        TI=1;  
        if(a>b) printf("%d\n",b);   
        else printf("%d\n",a);  
          
        TI=0;  
  
        //add from 1 to 10  
        c=0;    
        for (;i<11;i++) c = c + i;  
        TI=1;  
        printf("c=%d\n",c);    
        TI=0;  
        P1=i;  
  
        //get the area of circle 'a'  
        s=3.14*a*a;  
        TI=1;  
        printf("s=%.2f\n",s);  
        TI=0;  
        P1=s;  
    }  
}  
