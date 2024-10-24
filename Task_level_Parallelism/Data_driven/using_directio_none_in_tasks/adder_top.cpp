#include "adder_top.h"
void addInputs(mystream_in &in, mystream_in &dinB,stream_vld_in&vldIn,mystream_out &dout_Y,stream_vld_out & vldOut)
{
            #pragma HLS pipeline II=1
            int var = vldIn.read();
            if(var ==0)
            {
              vldOut.write(var);
              return;
            }
            int tmp1 =  in.read(); // Blocking_read
            int tmp2 =  dinB.read(); // Blocking read
            int out = tmp1+ tmp2;
            dout_Y.write(out);
            vldOut.write(1);
    }

void adder_top(mystream_in &din_A, mystream_in &din_B, mystream_out &dout_Y,stream_vld_in &vldIn, stream_vld_out & vldOut) {

    hls_thread_local hls::task t1(addInputs,din_A,din_B,vldIn,dout_Y,vldOut);

}

