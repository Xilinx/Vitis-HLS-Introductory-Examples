/*
 * Copyright 2021 Xilinx, Inc.
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

#include <iostream>
#include "ap_axi_sdata.h"
#include "hls_stream.h"
using namespace std;

#define DWIDTH 32
#define type ap_int<DWIDTH>
typedef hls::axis<type, 0, 0, 0> pkt;
void example(hls::stream<pkt > &A,
	     hls::stream<pkt> &B);
int main()
{
  int i=100;
  hls::stream<pkt> A, B;
  pkt tmp1, tmp2;
  tmp1.data = i;
  
  A.write(tmp1);
  example(A,B);

  B.read(tmp2);
  if (tmp2.data != 105)
  {
    cout << "ERROR: results mismatch" << endl;
    return 1;
  }
  else
  {
    cout << "Success: results match" << endl;
    return 0;
  }
}

