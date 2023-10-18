#include "ap_axi_sdata.h"
#include "ap_int.h"
#include "hls_stream.h"
#include <unistd.h>


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

	return 0;
}
