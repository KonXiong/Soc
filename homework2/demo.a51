;----------------------------------------------------
;File name: demo
;Description: 作业测试P107
;Author: 熊康
;Version: V1.0
;Date: 2022.4.5
;-----------------------------------------------------
LEN 	DATA 50H		;长度计数单元
HEXR 	DATA 51H		;定义HEXR存储单元
ASCR 	DATA 52H		;定义ASCR存储单元
I		DATA 55H		;定义待取数行下标
J		DATA 56H		;定义待取数列下标
H		DATA 58H		;定义高位（百位）
L		DATA 59H		;定义低位（个位和十位）
;TABLE DATA 0100H	;定义数据块首地址
ORG 0000H		
AJMP PRO1
ORG 0100H
;6、用汇编语言编写程序，将R0中的8位二进制数的各位用其ASC 码表示，结果保存到片内RAM的30H开始的单元中。
PRO1:	MOV R0, #28H
		MOV R1, #30H
		MOV R7, #08H
		MOV A, R0
LOOP:	RLC A
		JC ONE
		MOV @R1, #30H
		JMP EXIT
ONE:	MOV @R1, #31H
EXIT: 	INC R1
		DJNZ R7, LOOP
		SJMP PRO2
			
;7.片内RAM的HEXR开始的单元中存放着一组十六进制数（一个单元放两位），数据的个数放在片内RAM的LEN单元中，用汇编语言编写程序将这些十六进制数据转换成ASCII码，并存入片内RAM中ASCR开始的单元。
PRO2:	MOV HEXR, #32H
		MOV LEN, #01H		;数据个数
		MOV DPTR, #TABLE2	;取表头地址
		MOV R2,	LEN			;(R2)<-(LEN)存数据个数
		MOV R1, #HEXR		;R1<-HEXR 存16进制数
		MOV R0, #ASCR 		;R0<-ASCR 存R1对应ASCII码
LOOP2:	MOV A, @R1			;A<-((R1))
		ANL A, #0F0H			;A<-(A)&0FH  取高位
		SWAP A				;高4位和低4位互换
		MOVC A, @A+DPTR		;A<-((A)+(DPTR))
		MOV @R0, A 			;R0<-A
		INC R0 				;R0右移一位
		MOV A, @R1			;A<-((R1))
		ANL A, #0FH			;A<-(A)&F0H  取低位
		MOVC A, @A+DPTR		;A<-((A)+(DPTR))
		MOV @R0, A			;R0<-A
		INC R0				;R0右移一位
		INC R1				;R1右移一位
		JMP EXIT2
EXIT2:	DJNZ R2, LOOP2		;若R2~=0，跳转到LOOP2
		SJMP PRO3
TABLE2:	DB 30H,31H,32H,33H,34H,35H,36H,37H,38H,39H,41H,42H,43H,44H,45H,46H
			
	
;8.程序存储器中有一个5行×8列的表格，用汇编语言编程把行下标位I、列下标位J的元素读入到累加器A中。
PRO3:	MOV DPTR, #TABLE3
		MOV I, #03H
		MOV J, #04H
		MOV A, I			;行偏移量
		MOV B, #08H
		MUL AB				;从起点到第I行第1列经过A*B个单元
		ADD A, J  			;列偏移量
		DEC A				;消除指针指向表头的位置偏差
		MOVC A, @A+DPTR
		JMP EXIT3
EXIT3:	SJMP PRO4
TABLE3:	DB 01H,02H,03H,04H,05H,06H,07H,08H,09H,0AH,11H,12H,13H,14H,15H,16H,17H,18H,19H,1AH,21H,22H,23H,24H,25H,26H,27H,28H,29H,2AH,31H,32H,33H,34H,35H,36H,37H,38H,39H,3AH,41H,42H,43H,44H,45H,46H,47H,48H,49H,4AH

;9.用汇编语言编写程序，将累加器A中的8位二进制数转换成为十进制数存放在片内RAM的21H（百位）和20H（十位和个位）单元中。
PRO4:	MOV A, #0A2H ;0010_1010
		MOV B, #0AH
		DIV AB
		MOV L, B
		MOV B, #0AH
		DIV AB
		MOV H, A
		MOV A, B
		SWAP A
		ADD A, L
		MOV L, A
		JMP EXIT4
EXIT4:	SJMP PRO5

;10.用汇编语言编写程序实现图3-8所示的硬件逻辑功能。其中P1.1、P1.2和P1.3分别是端口线上的信息，IE0，IE1为外部中断请求标志，25H和26H为两个位地址。
PRO5:	MOV	ACC.0, P1.1
		MOV ACC.1, P1.2
		MOV ACC.2, IE1
		ORL ACC.1, IE0
		ORL ACC.2, 25H
		MOV ACC.3, 26H
		CPL ACC.3
		ANL ACC.0, ACC.1
		ANL ACC.0, ACC.2
		ANL ACC.0, ACC.3
		MOV P1.3, ACC.0
		JMP EXIT5
EXIT5:	SJMP PRO6

;11.用汇编语言编程求两个无符号数据块中最大值的乘积。数据块的首地址分别位片内RAM的60H和70H，每个数据块的第一字节用来存放数据块的长度。结果存入片内RAM的5FH和5EH单元中，要求求数据块中的最大值，用子程序实现。
PRO6:	MOV R0, #60H
		ACALL MAX
		MOV R7, A		;第一个数据块最大值
		MOV R0, #70H
		ACALL MAX
		MOV B, R7
		MUL AB		;两个数据块最大值相乘
		MOV 5FH, B
		MOV 5EH, A
		SJMP $
MAX:  	MOV A,@R0	;数据块长度
		DEC A
		MOV R7, A
		INC R0
		MOV A,@R0	;第一个数据
LOOP6: 	CLR C
		MOV B,A
		INC R0
		MOV A, @R0
		SUBB A,B	;两个比较，也可以用CJNE实现
		JNC NEXT
		MOV A,B
		SJMP NEXT1
NEXT: 	ADD A, B
NEXT1:	DJNZ R7, LOOP6
		RET


END
		