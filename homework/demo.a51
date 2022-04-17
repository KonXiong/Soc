;----------------------------------------------------
;File name: demo
;Description: 作业测试P107
;Author: 熊康
;Version: V1.0
;Date: 2022.4.5
;-----------------------------------------------------
		DATA0 EQU 0FH
		X EQU 35H
		Y EQU 36H
		H EQU 31H
		L EQU 30H
		TEMP EQU 37H
		ORG 0000H
		AJMP PRO1
		ORG 0100H
			
;T1片外RAM从1000H~10FFH单元有一数据块，用汇编语言编写程序将其传送到片外RAM的2500H单元开始的区域中。
PRO1:	CLR C
		MOV R0,#0
LOOP:	MOV P2,#10H
		MOVX A,@R0
		MOV P2,#25H
		MOVX @R0,A
		DJNZ R0,LOOP
		AJMP PRO2

;T2用汇编语言编写将片内RAM的31H、30H单元中的16位二进制数（31H中位高位）求补码后放回原单元的程序
PRO2:	CLR C
		MOV L, #10H
		MOV H, #20H
		MOV A, H
		JNB ACC.7, POS
		CPL A
		MOV H, A
		MOV A, L
		CPL A
		ADDC A, #01H
		MOV L, A
		MOV A, H
		ADDC A, #00H
		MOV H, A
POS:	AJMP PRO3

;T3用汇编语言编写将累加器A的一位十六进制数（A的高4位为0）转换为ASCII码的程序，转换结果仍存放在累加器A中，要求用查表和非查表两种方式实现。
PRO3:
;查表
START1:	MOV A, #0FH
		MOV DPTR, #TAB
		MOVC A,@A+DPTR
		AJMP START2
TAB: 	DB 30H,31H,32H,33H,34H,35H,36H,37H,38H,39H,41H,42H,43H,44H,45H,46H
		
;无查表，数字从48（30H）开始，大写字母从65（41H）开始		
START2:	MOV A, #0DH
		ADD A, #30H
		CJNE A, #3AH, CONT
CONT:	JNC ADD7
		JMP EXIT
ADD7:	ADD A, #07H
EXIT:	AJMP PRO4
		
;T4用汇编语言编程实现函数，设x的值存放在片内RAM的35H单元，y的值存放在片内RAM的36H单元。y = x+1(x>10)+0(5<=x<=10)-1(x<5)	
PRO4:	CLR C
		MOV 35H, #0BH
		MOV A, X   
		SUBB A, #0BH
		JNB ACC.7, BIG  	;与11比较，大于等于11，转BIG
		MOV A, 35H
		SUBB A, #05H
		JNB ACC.7, MID		;与5比较，大于等于5，转MID
		MOV A, #0FFH		;小于5的，A=-1（补码为0FFH）
BIG:	MOV A, X
		ADD A, #01H
		LJMP DONE
MID:	MOV A, X
		MOV A, #00H
		LJMP DONE
DONE:	MOV Y, A
		AJMP PRO5

;T5假设累加器A中的内容0~5，编写根据累加器A的不同内容，转向不同分支进行处理的汇编语言程序。		
PRO5:	CLR C
		MOV TEMP, #03H
		MOV A, TEMP
		SUBB A, #05H
		JZ BRANCH5
		MOV A, TEMP
		SUBB A, #03H		;在上一个SUBB执行后，CY=1
		JZ BRANCH4
		MOV A, TEMP
		SUBB A, #02H
		JZ BRANCH3
		MOV A, TEMP
		SUBB A, #01H
		JZ BRANCH2
		MOV A, TEMP
		SUBB A, #00H
		JZ BRANCH1
		AJMP BRANCH0
BRANCH0:MOV A, TEMP
		ADD A, #02H
		LJMP LAST
BRANCH1:MOV A, TEMP
		SUBB A, #02H
		LJMP LAST
BRANCH2:MOV A, TEMP
		ANL A, #10H
		LJMP LAST
BRANCH3:MOV A, TEMP
		RL A
		LJMP LAST
BRANCH4:MOV A, TEMP
		CPL A
		LJMP LAST
BRANCH5:MOV A, TEMP
		INC A
		LJMP LAST
LAST:	SJMP $
		END
			

