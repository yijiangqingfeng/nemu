#include "cpu/exec/template-start.h"

#define instr scas

static void do_execute(){
	if(ops_decoded.is_stack_size_16){
		DATA_TYPE rst = reg_w(R_AX)-reg_w(R_DI);
		int len = (DATA_BYTE << 3) -1;
		cpu.CF = op_dest -> val < op_src -> val;
		cpu.SF = rst >> len;
		int s1,s2;
		s1 = op_dest -> val >> len;
		s2 = op_src -> val >> len;
		cpu.OF = (s1!=s2 && s2 == cpu.SF);
		cpu.ZF = !rst;
		rst ^= rst >> 4;
		rst ^= rst >> 2;
		rst ^= rst >> 1;
		cpu.PF = !(rst & 1);
		if(cpu.DF==0){
			reg_w(R_DI)+=DATA_BYTE;
		}else{
			reg_w(R_DI)-=DATA_BYTE;
		}
	}else{
		DATA_TYPE rst = reg_w(R_AX)-reg_w(R_DI);
		//swaddr_write(reg_l(R_EAX),4,swaddr_read(reg_l(R_EDI),4));
		int len = (DATA_BYTE << 3) -1;
		cpu.CF = reg_w(R_AX)<reg_w(R_DI);
		cpu.SF = rst >> len;
		int s1,s2;
		s1 = op_dest -> val >> len;
		s2 = op_src -> val >> len;
		cpu.OF = (s1!=s2 && s2 == cpu.SF);
		cpu.ZF = !rst;
		rst ^= rst >> 4;
		rst ^= rst >> 2;
		rst ^= rst >> 1;
		cpu.PF = !(rst & 1);
		if(cpu.DF==0){
			reg_l(R_EDI)+=DATA_BYTE;
		}else{
			reg_l(R_EDI)-=DATA_BYTE;
		}
	}
	print_asm("scas");
}
make_instr_helper(n)
#include "cpu/exec/template-end.h"
