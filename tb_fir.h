#ifndef TB_FIR_H
#define TB_FIR_H
#include "define.h"
SC_MODULE(tb_fir)
{
	sc_in<bool> clk;
	sc_in<bool> reset_n;
	sc_in<sc_uint<24> > data_out;
	sc_out<sc_uint<8> > data_in;
	sc_out<sc_int<8> > coef[FILTER_TAPS];
	//file ptrs
	FILE *data_in_file, *coef_file, *data_out_golden_file;
	FILE *data_out_file, *data_out_read_file, *diff_file;
	//functions
	void send();
	void recv();
	void compare_results();
	SC_CTOR(tb_fir)
	{
		SC_CTHREAD(send,clk.pos());
		reset_signal_is(reset_n,false);
		SC_CTHREAD(recv,clk.pos());
		reset_signal_is(reset_n,false);
	}
	~tb_fir(){};
};
#endif
