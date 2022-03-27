;---------------------------------------
;File name: huibian
;Description: 汇编测试
;Author: 熊康
;Version: V1.0
;Date: 2022.3.27
;---------------------------------------

;$INCLUDE(C8051F020.INC) ;定义特殊寄存器
DATA0 EQU 12H
DATA1 EQU 34H
DATA2 EQU 56H	
X EQU 30H
Y EQU 31H

ORG 0000H
AJMP PRO1
ORG 0100H

PRO1:
;两数相加
	MOV A, #DATA0
	MOV B, #DATA1
	ADD A, B
	AJMP PRO2


PRO2:
;两数比大小
	MOV A, #DATA0
	MOV B, #DATA1
	SUBB A, B
	JNC DONE2
	ADD A, B ;(A)<=(B)
	AJMP ELSE2
	ELSE2:	MOV A, #DATA1   ;(A)>(B)
	DONE2:	AJMP PRO3
	

PRO3:
;累加1+2+3+……+10
	MOV R0, #10		;循环长度计数单元R0从15减到0
	MOV A, #0   	;累加结果先清0
	MOV R2, #0		;结果的低位
	MOV R3, #0		;结果的高位
	
	LOOP:
		
		MOV A, R2
		ADD A, R0
		MOV R2, A
		
		MOV A, R3
		ADDC A, #0
		MOV R3, A
		DJNZ R0, LOOP
		
		AJMP PRO4
		

PRO4: 
;符号函数
	MOV X, #0FEH
	MOV A, X
	JZ DONE				;若X=0，转DONE
	JNB ACC.7, POSI  	;若X>0，则转POSI
	MOV A, #0FFH		;若X<0，则A=-1（补码为0FFH）
	LJMP DONE
	POSI:	MOV A, #02H	;保存结果
	DONE:	MOV Y, A
			SJMP $
			
END
