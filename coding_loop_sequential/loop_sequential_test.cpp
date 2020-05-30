/*
 * Copyright 2020 Xilinx, Inc.
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

#include "loop_sequential.h"
 
int main () {
  din_t A[N], B[N];
  dout_t X[N], Y[N];
  dsel_t xlimit, ylimit;
  
  int i, retval=0;
  ofstream FILE;
  
  // Create input data
  for(i=0; i<N;++i) {
    A[i]=i;
    B[i]=N-1-i;
  }
  // Save the results to a file
  FILE.open ("result.dat");
  
  // Call the function
  xlimit=31;
  ylimit=31;
  loop_sequential(A,B,X,Y,xlimit,ylimit);
  
  for(i=0; i<N-1;++i) {
    FILE << X[i] << " " << Y[i] << endl;
  }
  FILE.close();
  
  // Compare the results file with the golden results
  retval = system("diff --brief -w result.dat result.golden.dat");
  if (retval != 0) {
    cout << "Test failed  !!!" << endl; 
    retval=1;
  } else {
    cout << "Test passed !" << endl;
  }
  
  // Return 0 if the test passed
  return retval;
}

