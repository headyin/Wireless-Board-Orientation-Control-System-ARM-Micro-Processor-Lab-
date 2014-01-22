	AREA enable_fpu_proc, CODE, READONLY
	;Function used to enable FPU
	EXPORT EnableFPU
EnableFPU
	;CPACR is located at address 0xE000ED88
	LDR.W	R0, =0xE000ED88
	;Read CPACR
	LDR		R1, [R0]
	;set bits 20-23 to enable CP10 and CP11 coprocessors
	ORR		R1, R1, #(0xF << 20)
	;Write back the modified value to the CPACR
	STR		R1, [R0]; wait for store to complete
	DSB
	;reset pipeline now the FPU is enabled
	ISB
	BX LR
	END