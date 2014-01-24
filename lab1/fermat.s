	AREA fermat_proc, CODE, READONLY
	;implement the fermat function
	;get N from stack, return R0, R1 to stack
	EXPORT Fermat
	IMPORT CeilSqrt
Fermat
	POP		{R0}	;get parameter N
	PUSH 	{LR}	;put the origin link register value to stack
	;if N < 1
	CMP		R0, #1
	BMI		Negative_Zero_Condition
	;if N is even
	ANDS	R1, R0, #1
	BEQ		Even_Condition
	
Odd_Condition
	MOV		R3, R0	;save N to R3, since R0 will be modified in ceilSqrt
	;call ceilSqrt function R1 = ceilSqrt(R0)
	PUSH	{R0}	;put paratemter to stack
	LDR		R0, =CeilSqrt
    BLX		R0
	POP		{R1}
	;y^2 = X^2 - N
	MUL		R2, R1, R1
	SUB		R2, R2, R3
	
Check_YY_is_square
	;check if (R2 - ceilSqrt(R2)^2 == 0)
	PUSH	{R2}
	LDR		R0, =CeilSqrt
    BLX		R0
	POP		{R4}
	MUL		R5, R4, R4
	CMP		R5, R2
	BEQ		YY_is_squire
	
YY_is_not_square
	;Y^2 += (2X + 1)
	LSL		R4, R1, #1
	ADD		R4, R4, #1
	ADD		R2, R2, R4
	;X +=1
	ADD		R1, R1, #1
	B		Check_YY_is_square

YY_is_squire
	ADD		R0, R1, R4	;R0 = x + y
	SUB		R1,	R1, R4	;R1 = x - y
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
	POP		{LR}
	PUSH	{R0,R1}
	BX LR ;
	END