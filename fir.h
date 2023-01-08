#ifndef FIR_H
#define FIR_h
#include "define.h"

SC_MODULE(fir)
{
	sc_in<bool> clk;
	sc_in<bool> reset_n;
	sc_in<sc_uint<8> > data_in;
	sc_in<sc_int<8> > coef[FILTER_TAPS];
	sc_out<sc_uint<24> > data_out;

	//functions
	void fir_main(void);
	sc_uint<24> filter(sc_uint<8> *, sc_int<8> *);
	SC_CTOR(fir)
	{
		SC_CTHREAD(fir_main,clk.pos());
		reset_signal_is(reset_n,false);
	}
	~fir(){};
};
#endif
