#include "ap_axi_sdata.h"
#include "ap_int.h"
#include "hls_stream.h"
#include <unistd.h>

<<<<<<< HEAD
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
=======

extern "C" void stable_pointer(int* mem, hls::stream<int>&, hls::stream<int>&);


int main(int argc, char *argv[])
{
	int memory[512];
	//memset(memory, 0, 128*256/8);
	hls::stream<int> in, out;
	int val;
	for(int i = 0; i < 100; i++) {
		in.write(i);
        memory[i] = 0;
	}

    stable_pointer(memory, in, out);

	for(int i = 0; i < 100; i++) {
		out.read(val);
        if (val != i * 23 * 11) {
            printf("%d != %d\n", val, i * 23 * 11);
            return 1;
        }
	}

>>>>>>> 91ac5ad8572ae97b46dbed8a5aa0280dc05ca2c4
	return 0;
}
