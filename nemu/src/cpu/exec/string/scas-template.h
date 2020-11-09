#include "cpu/exec/template-start.h"

#define instr scas

static void do_execute(){
	DATA_TYPE dest = REG(R_EAX);
	DATA_TYPE src = swaddr_read(reg_l(R_EDI),DATA_BYTE);
	DATA_TYPE rst = dest - src ;
	if(cpu.DF==0)reg_l(R_EDI) += DATA_BYTE;
	else reg_l(R_EDI) -= DATA_BYTE;
	print_asm("scas");
	if(dest<src)cpu.CF=1;
	int a = op_dest -> val >> (DATA_BYTE*8-1);
	int b = op_src -> val >> (DATA_BYTE*8-1);
	cpu.OF=(a!=b)&&(b==cpu.SF);
	cpu.ZF=!rst;
	int s = 0;
	int i;
	for(i = 0;i < 8;i++)s+=(rst>>i)&1;
	if(s % 2)cpu.PF = 0;
	else cpu.PF = 1;
	if(((op_dest->val&0xf)-(op_src->val &0xf))>>4)cpu.AF=1;
	else cpu.ZF=0;
}
make_instr_helper(n)
#include "cpu/exec/template-end.h"
