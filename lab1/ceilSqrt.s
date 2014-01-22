	AREA ceil_sqrt_proc, CODE, READONLY
	;calculate the ceil of square root (N)
	;N is an signed 32 bit integer, stored in the stack
	EXPORT CeilSqrt
CeilSqrt
	VPOP.F32		S1					;get paremeter N from stack
	VCVT.F32.S32 	S0, S1				;convert int32 to float32
	VSQRT.F32		S1, S0				;calculate squart root
	MOV 			R0, 0x00400000  	;set FPSCR tp RP mode, round to plus infinity
	VMSR 			FPSCR, R0
	VCVTR.S32.F32 	S0, S1				;convert float32 to int32
	VPUSH 			{S0}				;push result to stack
	BX LR ;
	END