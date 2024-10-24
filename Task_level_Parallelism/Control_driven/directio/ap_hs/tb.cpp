/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "dut.h"
using namespace std;

int main(){
	hls::ap_hs<int> reset_value;
	hls::ap_hs<int> reset_myCounter;
    hls::stream<packet> in, out;
    int mem[DATA];
    for(int i =0;i<4999;i++)
    {
        reset_myCounter.write(0);
    }
    reset_myCounter.write(1);

    packet tmp1, tmp2,tmp3;
  //  hls::stream<packet> in, out;
    for (unsigned i = 0; i < 5000; i++) {
        tmp1.data = i;
        tmp1.keep = -1;
        tmp1.strb = 1;
        tmp1.user = 1;
          
            if(i==DATA-1)
                tmp1.last = 1;
            else 
                tmp1.last =0;
        in.write(tmp1);
        mem[i] = i;
    }   
    int err=0;

krnl_stream_vdatamover(in,out,mem,reset_value,reset_myCounter);
    vector<int> reset_mem;
 for (unsigned i = 0; i < 5000; i++) {
        out.read(tmp2);
        reset_mem.push_back(reset_value.read());
        if(tmp2.data != i*2)
        {
            cout << "ERROR: results mismatch" << endl;
            cout << "i *2 =" << i*2;
            cout << " != ";
            cout << "tmp2.data=" << tmp2.data << endl;
            err=1;
            break;
        }
    }
    for(int i=0;i<reset_mem.size();i++)
    {
        if(i == reset_mem.size()-1)
        {
            if(reset_mem[i] !=0)
            {
                cout << "ERROR during the threshold value = " << reset_mem[i] ;
                err=1;
                break;
            }
        }
    }
	return err;
}

