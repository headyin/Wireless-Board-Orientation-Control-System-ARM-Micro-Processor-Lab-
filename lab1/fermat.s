	AREA fermat_proc, CODE, READONLY
	;implement the fermat function
	;get N from stack, return R0, R1 to stack
	;uint64_t Fermat(uint32_t n);
	EXPORT fermat
	IMPORT ceilSqrt
	IMPORT roundSqrt
fermat
	PUSH 	{R4-R12, LR}	;put the origin link register value to stack
	;if N < 1
	CMP		R0, #1
	BMI		Negative_Zero_Condition
	;if N is even
	ANDS	R1, R0, #1
	BEQ		Even_Condition
	
Odd_Condition
	MOV		R4, R0	;save N to R3, since R0 will be modified in ceilSqrt
	;call ceilSqrt function R1 = ceilSqrt(R0)
    BL		ceilSqrt
	MOV		R5, R0	;save X to R5
	;y^2 = X^2 - N save Y^2 to R6
	MUL		R6, R5, R5
	SUB		R6, R6, R4 
	
Check_YY_is_square
	;check if (R6 - ceilSqrt(R6)^2 == 0)
	MOV		R0, R6
    BL		roundSqrt
	MUL		R1, R0, R0
	CMP		R6, R1
	BEQ		YY_is_squire
	
YY_is_not_square
	;Y^2 += (2X + 1)
	LSL		R0, R5, #1
	ADD		R0, R0, #1
	ADD		R6, R6, R0
	;X +=1
	ADD		R5, R5, #1
	B		Check_YY_is_square

YY_is_squire
	MOV		R6,	R0		;now y is stored in R6
	ADD		R0, R5, R6	;R0 = x + y
	SUB		R1,	R5, R6	;R1 = x - y
	B		Fermat_return
	
Even_Condition
	;return 2 and N/2
	LSR		R1, R0, #1
	MOV		R0, #2
	B		Fermat_return

Negative_Zero_Condition
	;return 0 0
	MOV 	R0, #0
	MOV 	R1, #0
	B		Fermat_return
	
Fermat_return
	POP		{R4-R12, LR}
	BX 		LR
	END