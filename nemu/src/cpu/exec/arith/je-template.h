#include "cpu/exec/template-start.h"

#define instr je

make_helper(concat(je_i_,SUFFIX)){
    int len = concat(decode_i_,SUFFIX)(cpu.eip + 1);
    if(cpu.ZF==1)cpu.eip+=op_src -> val;
    print_asm("je\t%x",cpu.eip+1);
    return len + 1;
}

#include "cpu/exec/template-end.h"
