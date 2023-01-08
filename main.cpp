#include "define.h"
#include "fir.h"
#include "tb_fir.h"
#define WAVE_DUMP

int sc_main(int argc, char ** charv)
{
	sc_clock clk("clk",25,SC_NS,0.5,12.5,SC_NS,true);
	sc_signal<bool> reset_n;
	sc_signal<sc_uint<8> > data_in;
	sc_signal<sc_uint<24> > data_out;
	sc_signal<sc_int<8> > coef[FILTER_TAPS];
	int x;
	char name[15];
	//instantiations
	fir fir_inst("fit_inst");
	tb_fir tb_fir_inst("tb_fir_inst");
	//connections
	fir_inst.clk(clk);
	fir_inst.reset_n(reset_n);
	fir_inst.data_in(data_in);
	fir_inst.data_out(data_out);
	for(x=0;x<FILTER_TAPS;x++)
	{
		fir_inst.coef[x](coef[x]);
	}
	tb_fir_inst.clk(clk);
	tb_fir_inst.reset_n(reset_n);
	tb_fir_inst.data_in(data_in);
	tb_fir_inst.data_out(data_out);
	for(x=0;x<FILTER_TAPS;x++)
	{
		tb_fir_inst.coef[x](coef[x]);
	}
#ifdef WAVE_DUMP
	sc_trace_file* trace_file = sc_create_vcd_trace_file("trace_file");
	sc_trace(trace_file,clk,"clk");
	sc_trace(trace_file,reset_n,"reset_n");
	sc_trace(trace_file,data_in,name);

	sc_trace(trace_file,data_out,"data_out");
#endif
	sc_start(25,SC_NS);
	reset_n.write(0);
	sc_start(25,SC_NS);
	reset_n.write(1);
	sc_start();
#ifdef WAVE_DUMP
	sc_close_vcd_trace_file(trace_file);
#endif
	return 0;
}
