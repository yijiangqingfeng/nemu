#include "cpu/exec/template-start.h"

#define instr shl

static void do_execute () {
	DATA_TYPE src = op_src->val;
	DATA_TYPE dest = op_dest->val;
	int len = (DATA_BYTE << 3) - 1;

	uint8_t count = src & 0x1f;
	dest <<= count;
	OPERAND_W(op_dest, dest);
	DATA_TYPE rst = dest;
	cpu.CF = 0;
	cpu.OF = 0;
	cpu.ZF = !rst;
	cpu.SF=rst>>len;
	rst^=rst>>4;
	rst^=rst>>2;
	rst^=rst>>1;
	cpu.PF = !(rst & 1);
	/* TODO: Update EFLAGS. */
	//panic("please implement me");

	print_asm_template1();
}

make_instr_helper(rm_1)
make_instr_helper(rm_cl)
make_instr_helper(rm_imm)

#include "cpu/exec/template-end.h"
