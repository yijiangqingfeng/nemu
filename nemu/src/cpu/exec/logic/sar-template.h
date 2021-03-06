#include "cpu/exec/template-start.h"

#define instr sar

static void do_execute () {
	DATA_TYPE src = op_src->val;
	DATA_TYPE_S dest = op_dest->val;
	DATA_TYPE_S rst = op_dest->val;

	uint8_t count = src & 0x1f;
	dest >>= count;
	int len = (DATA_BYTE << 3) - 1;
	cpu.CF=0;
	cpu.ZF=0;
	cpu.SF=rst>>len;
	cpu.ZF=!rst;
	OPERAND_W(op_dest, dest);
	rst^=rst>>4;
	rst^=rst>>2;
	rst^=rst>>1;
	cpu.PF=!(rst&1);

	/* TODO: Update EFLAGS. */
	//panic("please implement me");

	print_asm_template2();
}

make_instr_helper(rm_1)
make_instr_helper(rm_cl)
make_instr_helper(rm_imm)

#include "cpu/exec/template-end.h"
