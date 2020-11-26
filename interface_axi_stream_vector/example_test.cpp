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

#include "example.h"

#include <cmath>
#include <iomanip>
#include <iostream>
using namespace std;

void example(hls::stream<float4> &a, hls::stream<float4> &b);

int main()
{
  hls::stream<float4> A, B;
  float4 sample_in, sample_out;

  // Put some Celsius numbers (by chunks of 4) into A
  for (int i = 0; i < 25; i++)
  {
    for (int j = 0; j < 4; j++)
      sample_in[j] = (float)(4 * i + j);
    A.write(sample_in);
  }

  //Call the hardware function
  cout << "Executing the function..." << endl;
  example(A, B);

  // Read B (the temperature in Farenheit)
  // then compare to expected results
  for (int i = 0; i < 25; i++)
  {
    B.read(sample_out);
    for (int j = 0; j < 4; j++)
    {
      cout << 4 * i + j << " Celsius is " << sample_out[j] << " Fahrenheit" << endl;
      if (floor(sample_out[j]) != floor((4 * i + j) * 1.8 + 32))
      {
        cout << "ERROR HW and SW results mismatch" << endl;
        return 1;
      }
    }
  }

  // No errors seen
  cout << "Success HW and SW results match!" << endl;
  return 0;
}
