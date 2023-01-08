#include "tb_fir.h"

void tb_fir::send()
{
	int i=0;
	unsigned int data_in_read;
	float coef_read;

	data_in_file = fopen(INFILTERFILENAME,"rt");
	if(!data_in_file)
	{
		cout<< "could not open file "<<INFILTERFILENAME<<endl;
		sc_stop();
		exit(-1);
	}
	coef_file = fopen(INCOEFFFILENAME,"rt");

	if(!coef_file)
	{
		cout<< "could not open file "<<INCOEFFFILENAME<<endl;
		sc_stop();
		exit(-1);
	}
	data_in.write(0);
	for(int i =0;i<FILTER_TAPS;i++)
	{
		if(fscanf(coef_file,"%f", &coef_read) == EOF)
			break;
		coef[i].write(sc_int<8>(coef_read));
	}
	wait();
	while(true)
	{
		while(fscanf(data_in_file,"%u", &data_in_read)!=EOF)
		{
			data_in.write(data_in_read);
			wait();
		}
		fclose(data_in_file);
		fclose(coef_file);
		cout<<endl<<"starting comparing the results"<<endl;
		compare_results();
		sc_stop();
		wait();
	}
}

void tb_fir::recv()
{
	unsigned int data_out_write = 0;
	data_out_file = fopen(OUTFILENAME,"wt");
	if(!data_out_file)
	{
		cout<< "could not open file "<<OUTFILENAME<<endl;
		sc_stop();
		exit(-1);
	}
	wait();
	while(true)
	{
		data_out_write = data_out.read();
		fprintf(data_out_file,"%d\n",data_out_write);
		wait();
	}
}

void tb_fir::compare_results()
{
	int data_out_golden_filter=0,data_out_filter=0,line=1,errors=0;
	fclose(data_out_file);
	data_out_file = fopen(OUTFILENAME,"rt");
	if(!data_out_file)
	{
		cout<< "could not open file "<<OUTFILENAME<<endl;
		sc_stop();
		exit(-1);
	}
	data_out_golden_file = fopen(OUTFILENAME_GOLDEN,"rt");
	if(!data_out_golden_file)
	{
		cout<< "could not open file "<<OUTFILENAME<<endl;
		sc_stop();
		exit(-1);
	}
	diff_file = fopen(DIFFFILENAME,"rt");
	if(!diff_file)
	{
		cout<< "could not open file "<<DIFFFILENAME<<endl;
		sc_stop();
		exit(-1);
	}
	while(fscanf(data_out_golden_file,"%d",&data_out_golden_filter)!= EOF)
	{
		fscanf(data_out_file,"%d",&data_out_filter);
		cout << endl <<"cycle["<< line << "]: " << data_out_filter << "-- "<< data_out_golden_filter;
		if(data_out_filter != data_out_golden_filter)
		{
			cout << "\noutput missmatch [line:" << line << "] golden:" << data_out_golden_filter << " -- output:" << data_out_filter;

			fprintf(diff_file,"\noutput missmatch[line:%d] golden: %d -- output: %d",line, data_out_golden_filter, data_out_filter);
			errors++;
		}
		line++;
	}
	if(errors == 0)
	{
		cout<<endl<<"simulation finished successfully"<<endl;
	}
	else
		cout<<endl<<"simulation failed"<<endl;

	fclose(data_out_file);
	fclose(data_out_golden_file);
	fclose(diff_file);
}

