#include "adder_top.h"

#define N 25
int main() {

    mystream_in in1;
    mystream_in in2;
    mystream_out out;
    stream_vld_in   vIn;
    stream_vld_out vOut;
    int scalarInput = 1;
    int golden[N];
    for (int i = 0; i < N; i++)
    {
        in1.write(i);
        in2.write(i);
        vIn.write(1);
        golden[i] = 2*i;
    }
     vIn.write(0);
     vIn.write(0);
     vIn.write(0);
    adder_top(in1, in2, out,vIn,vOut);

    for(int i=0;i<N;i++)
    {
        int var = vOut.read();
        if(var == 1)
        {   
            if(out.read() != golden[i])
                return 1;
        }
    }

    return 0;
}