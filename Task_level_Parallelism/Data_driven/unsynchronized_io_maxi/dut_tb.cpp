#include "ap_axi_sdata.h"
#include "ap_int.h"
#include "hls_stream.h"
#include <unistd.h>
#define DWIDTH 32
typedef ap_axiu<DWIDTH, 0, 0, 0> trans_pkt;

extern "C" void increment(hls::stream<trans_pkt >& input, hls::stream<trans_pkt >& output, int *out, int *in);


int main(int argc, char *argv[])
{	int size=1024;

	int memory_out[size];
	int memory_in[size];
	//memset(memory, 0, 128*256/8);
	hls::stream<trans_pkt> in, out;
	trans_pkt tmp;
	trans_pkt val;
	for(int i = 0; i < size; i++) {
		tmp.data = i;
		in.write(tmp);
		memory_out[i] = 0;
		memory_in[i] =0;
	}

    increment(in, out, memory_out,memory_in);

	for(int i = 0; i < 100; i++) {
		out.read(val);
			if(val.data != i*2)
			{
				printf("%d != %d\n", val.data, i );
				return 1;
			}
        }
	return 0;
}
