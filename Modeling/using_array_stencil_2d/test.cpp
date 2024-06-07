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

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <chrono>

#include "cmdlineparser.h" 
#include "coefficients.h"
#include "common.h" 

using namespace std;
using namespace sda;
using namespace sda::utils;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

int main(int argc, char** argv)
{
  printf("----------------------------------------------------------------------------\n");
  printf("HLS Testbench for Xilinx 2D Filter Example\n");
        
  // ---------------------------------------------------------------------------------
  // Parse command line
  // ---------------------------------------------------------------------------------
  CmdLineParser parser;
  parser.addSwitch("--width",  "-w", "Image width", "1000");
  parser.addSwitch("--height", "-h", "Image height",  "30");

  //parse all command line options
  parser.parse(argc, argv);
  unsigned width   = parser.value_to_int("width");
  unsigned height  = parser.value_to_int("height");
  unsigned stride  = width;//ceil(width/64.0)*64;
  unsigned nbytes  = stride*height; //(stride*height);

  printf("Image info\n");
  printf("- Width     : %10d\n", width);
  printf("- Height    : %10d\n", height);
  printf("- Stride    : %10d\n", stride);
  printf("- Bytes     : %10d\n", nbytes);

unsigned char y_src[MAX_IMAGE_HEIGHT][MAX_IMAGE_WIDTH];
unsigned char y_dst[MAX_IMAGE_HEIGHT][MAX_IMAGE_WIDTH];

  // Coefficients
  char coeffs[FILTER_V_SIZE][FILTER_H_SIZE];
  bool diff = false;

for (int s=0;s<2;s++)
{
  printf("Initializing the vectors\n");    

for (int y=0;y<MAX_IMAGE_HEIGHT;y++)
{
 for (int x=0; x<MAX_IMAGE_WIDTH; x++) {
  	y_src[y][x] = rand();
  	y_dst[y][x] = 0;

  }
}
  printf("Initializing the coefficients\n");    

  for(int row=0; row<FILTER_V_SIZE; row++) {
    for(int col=0; col<FILTER_H_SIZE; col++) {
      coeffs[row][col]=1;
    }
  }

  unsigned num_coefs = FILTER_V_SIZE*FILTER_H_SIZE;
  unsigned num_coefs_padded = (((num_coefs-1)/64)+1)*64;
  char *coeffs_padded = (char *)malloc(num_coefs_padded);
  memcpy(coeffs_padded, &coeffs[0][0], num_coefs);

  // filter factor and bias
  float factor = 1.0/(FILTER_V_SIZE*FILTER_H_SIZE);
  short bias   = 0;  

  // ---------------------------------------------------------------------------------
  // Run the FPGA kernels
  // ---------------------------------------------------------------------------------

  printf("Running FPGA accelerator\n");    
  Filter2DKernel(coeffs_padded,  y_src, y_dst);


  // ---------------------------------------------------------------------------------
  // Compute reference results and compare
  // ---------------------------------------------------------------------------------

  printf("Comparing results\n");    

  // Create output buffers for reference results
  unsigned char y_ref[MAX_IMAGE_HEIGHT][MAX_IMAGE_WIDTH];
  // Compute reference results
   Filter2D(coeffs, factor, bias, width, height, stride, y_src, y_ref);
 
  // Compare results
  for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
          if ( y_dst[y][x] != y_ref[y][x] ) diff = true;
      }
  }
/**/
  if(diff) {
      printf("%sTest FAILED: Output has mismatches with reference%s\n", RED, RESET);
      return 1;    
  } else {
      printf("%sTest PASSED: Output matches reference%s\n", GREEN, RESET);    
  }
}
  printf("----------------------------------------------------------------------------\n"); 

  return (diff?1:0);
}


