	AREA round_sqrt_proc, CODE, READONLY
	;calculate the round of square root (N)
	;N is an signed 32 bit integer, stored in R0
	;return value through R0
	EXPORT roundSqrt
roundSqrt
	VMOV 			S0, R0
	VCVT.F32.S32 	S0, S0				;convert int32 to float32
	VSQRT.F32		S1, S0				;calculate squart root
	MOV 			R0, 0x00000000  	;set FPSCR tp RP mode, round to nearest
	VMSR 			FPSCR, R0
	VCVTR.S32.F32 	S0, S1				;convert float32 to int32
	VMOV			R0, S0
	BX LR ;
	END