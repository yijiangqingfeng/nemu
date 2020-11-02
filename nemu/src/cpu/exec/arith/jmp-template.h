#include "cpu/exec/template-start.h"

#define instr jmp

make_helper(concat(jmp_i_, SUFFIX)){
    int len = concat(decode_i_, SUFFIX)(cpu.eip + 1);
    DATA_TYPE_S imm = op_src -> val;
    print_asm("jmp\t%x",cpu.eip+1+len+imm);
    cpu.eip += imm;
    return len + 1;
}

#include "cpu/exec/template-end.h"
