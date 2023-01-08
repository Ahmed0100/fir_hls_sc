#include "fir.h"
#define MAX 255
void fir::fir_main(void)
{
	sc_uint<24> filter_output_function;
	sc_uint<8> data_in_read;
	sc_int<8> coef_read[FILTER_TAPS];
	sc_uint<8> data_buffer[FILTER_TAPS];

	for(int i=0;i<FILTER_TAPS;i++)
		data_buffer[i] = 0;
	wait(); // reset state
	while(1)
	{
		data_in_read = data_in.read();
		for(int i=0;i<FILTER_TAPS;i++)
			coef_read[i] = coef[i].read();
		for(int i=FILTER_TAPS-1;i>0;i--)
		{
			data_buffer[i] = data_buffer[i-1];
		}
		data_buffer[0] = data_in_read;

		filter_output_function = filter(data_buffer,coef_read);
		data_out.write(filter_output_function);
		wait();
	}
}
sc_uint<24> fir::filter(sc_uint<8> *data_buffer,
		sc_int<8> *coef)
{
	sc_int<25> sop=0;
	sc_uint<24> filter_result;
	int i;
	for(int i=0;i<FILTER_TAPS;i++)
		sop += sc_int<25>(coef[i]*data_buffer[i]);
	if(sop <0)
		sop = 0;
	filter_result = sc_uint<24>(sop);
	return filter_result;
}
