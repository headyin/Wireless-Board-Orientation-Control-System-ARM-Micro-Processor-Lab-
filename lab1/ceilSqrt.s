	AREA ceil_sqrt_proc, CODE, READONLY
	;calculate the ceil of square root (N)
	;N is an signed 32 bit integer, stored in R0
	;return value through R0
	EXPORT ceilSqrt
ceilSqrt
	VMOV 			S0, R0
	VCVT.F32.U32 	S0, S0				;convert int32 to float32
	VSQRT.F32		S1, S0				;calculate squart root
	MOV 			R0, #0x00400000  	;set FPSCR tp RP mode, round to plus infinity
	VMSR 			FPSCR, R0
	VCVTR.U32.F32 	S0, S1				;convert float32 to int32
	VMOV			R0, S0
	BX LR ;
	END