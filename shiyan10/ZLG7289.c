#include "C8051F020.H"
#include "ZLG7289.h"
//����һ��ȫ�ֱ�����������ʱ
//�ñ����ڵ��ú���ZLG7289_Init()ʱ����ʼ��
unsigned char ZLG7289_Delay_t;
/*
�궨�壺ZLG7289_ShortDelay()
���ܣ�����ʱ
˵������ʱ(ZLG7289_Delay_t*2+2)����������
*/
#define ZLG7289_ShortDelay() \
{\
unsigned char t = ZLG7289_Delay_t;\
while ( --t != 0 );\
}
/*
�궨�壺ZLG7289_LongDelay()
���ܣ�����ʱ
˵������ʱ(ZLG7289_Delay_t*12+8)����������
*/
#define ZLG7289_LongDelay() \
{\
unsigned char t = ZLG7289_Delay_t * 6;\
while ( --t != 0 );\
}
/*
������ZLG7289_SPI_Write()
���ܣ���SPI ����д��1 ���ֽڵ�����
������
dat��Ҫд�������
*/
void ZLG7289_SPI_Write(char dat)    // reentrant//ȥ�����������ԣ������������������
{
unsigned char t = 8;
do
{
ZLG7289_pinDIO = (bit)(dat & 0x80);
dat <<= 1;
ZLG7289_pinCLK = 1;
ZLG7289_ShortDelay();
ZLG7289_pinCLK = 0;
ZLG7289_ShortDelay();
} while ( --t != 0 );
}
/*
������ZLG7289_SPI_Read()
���ܣ���SPI ���߶�ȡ1 ���ֽڵ�����
���أ�
��ȡ��������
*/
char ZLG7289_SPI_Read()      //reentrant //   ȥ�����������ԣ������������������
{
char dat;
unsigned char t = 8;
ZLG7289_pinDIO = 1; //��ȡ����֮ǰDIO ����Ҫ��1 ���л�������״̬
do
{
ZLG7289_pinCLK = 1;
ZLG7289_ShortDelay();
dat <<= 1;
if ( ZLG7289_pinDIO ) dat++;
ZLG7289_pinCLK = 0;
ZLG7289_ShortDelay();
} while ( --t != 0 );
return dat;
}
/*
������ZLG7289_cmd()
���ܣ�ִ��ZLG7289 ��ָ��
������
cmd��������
*/
void ZLG7289_cmd(char cmd)
{
char sav = IE;
IE &= 0xFA; //�ر��ⲿ�ж�
ZLG7289_pinCS = 0;
ZLG7289_LongDelay();
ZLG7289_SPI_Write(cmd);
ZLG7289_pinCS = 1;
ZLG7289_LongDelay();
IE = sav; //�ָ�IE
}
/*
������ZLG7289_cmd_dat()
���ܣ�ִ��ZLG7289 ������ָ��
������
cmd��������
dat������
*/
void ZLG7289_cmd_dat(char cmd, char dat)
{
char sav = IE;
IE &= 0xFA; //�ر��ⲿ�ж�
ZLG7289_pinCS = 0;
ZLG7289_LongDelay();
ZLG7289_SPI_Write(cmd);
ZLG7289_LongDelay();
ZLG7289_SPI_Write(dat);
ZLG7289_pinCS = 1;
ZLG7289_LongDelay();
IE = sav; //�ָ�IE
}
/*
������ZLG7289_Download()
���ܣ���������
������
mod=0�����������Ұ���ʽ0 ����
mod=1�����������Ұ���ʽ1 ����
mod=2���������ݵ�������
x������ܱ�ţ������꣩��ȡֵ0��7
dp=0��С���㲻��
dp=1��С������
dat��Ҫ��ʾ������
˵����
���뷽ʽ�ľ��庬����ο������ֲ�
*/
void ZLG7289_Download(unsigned char mod, char x, bit dp, char dat)
{
code char ModDat[3] = {0x80,0xC8,0x90};
char d1;
char d2;
if ( mod > 2 ) mod = 2;
d1 = ModDat[mod];
x &= 0x07;
d1 |= x;
d2 = dat & 0x7F;
if ( dp ) d2 |= 0x80;
ZLG7289_cmd_dat(d1,d2);
}
/*
���ܣ�ִ��ZLG7289 ��������
���أ�
���ض����İ���ֵ��0��63
�������0xFF ���ʾû�м�����
*/
char ZLG7289_Key()      //reentrant  //ȥ�����������ԣ������������������
{
char key;
ZLG7289_pinCS = 0;
ZLG7289_LongDelay();
ZLG7289_SPI_Write(0x15);
ZLG7289_LongDelay();
key = ZLG7289_SPI_Read();
ZLG7289_pinCS = 1;
ZLG7289_LongDelay();
return key;
}
/*
������ZLG7289_Init()
���ܣ�ZLG7289 ��ʼ��
������
t��SPI ���ߵ���ʱֵ���ã�ȡֵ1-40��������Χ���ܵ��´���
˵����
t��ȡֵ���Բ������¹�ʽ��t >= 5 * f1 / f2������f1 ��ʾ80C51 ��
����Ƶ�ʣ�f2 ��ʾZLG7289 �ľ���Ƶ��
*/
void ZLG7289_Init(unsigned char t)
{
unsigned char x;
//I/O �ڳ�ʼ��
ZLG7289_pinCS = 1;
ZLG7289_pinCLK = 0;
ZLG7289_pinDIO = 1;
ZLG7289_pinINT = 1;
//��ʱ��ʼ��
ZLG7289_Delay_t = t;
//�������������
for ( x=0; x<8; x++ )
{
ZLG7289_Download(1,x,1,8);
}
}

