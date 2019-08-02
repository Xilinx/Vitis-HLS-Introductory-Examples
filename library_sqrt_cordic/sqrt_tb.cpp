/*****************************************************************************
 *
 *     Author: Xilinx, Inc.
 *
 *     This text contains proprietary, confidential information of
 *     Xilinx, Inc. , is distributed by under license from Xilinx,
 *     Inc., and may be used, copied and/or disclosed only pursuant to
 *     the terms of a valid license agreement with Xilinx, Inc.
 *
 *     XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
 *     AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND
 *     SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,
 *     OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,
 *     APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION
 *     THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
 *     AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
 *     FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
 *     WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
 *     IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
 *     REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
 *     INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *     FOR A PARTICULAR PURPOSE.
 *
 *     Xilinx products are not intended for use in life support appliances,
 *     devices, or systems. Use in such applications is expressly prohibited.
 *
 *     (c) Copyright 2014-2019 Xilinx Inc.
 *     All rights reserved.
 *
 *****************************************************************************/

#include "sqrt.h"
#include <cmath>

int main (void){

  const int SAMPLES = 10;
  const ap_uint<OutputWidth> TOLERANCE = 0;

  hls::sqrt_input<InputWidth, DataFormat>::in x_val;
  hls::sqrt_input<InputWidth, DataFormat>::in reference_x_val;
  hls::sqrt_output<OutputWidth, DataFormat>::out sqrtX_val;
  ap_uint<OutputWidth> expected_sqrtX;
  ap_uint<OutputWidth> difference;

  hls::stream< hls::sqrt_input<InputWidth, DataFormat>::in > x("Input Data: x");
  hls::stream< hls::sqrt_input<InputWidth, DataFormat>::in > reference_x("Reference Input Data: x");
  hls::stream< hls::sqrt_output<OutputWidth, DataFormat>::out > sqrtX("Output Data: sqrt(x)");

  hls::sqrt_input<InputWidth, DataFormat>::in x_sample;
  hls::sqrt_output<OutputWidth, DataFormat>::out sqrtX_sample;

  int fail = 0;

  // Generate input data
  for (int sample=0; sample<=SAMPLES; sample++) {
    x_val.in = ((1 << sample)*sample);
    x << x_val;
    reference_x << x_val;
  }

  // Call wrapped square root
  while (!x.empty()) {
    x.read(x_sample);
    sqrt_top(x_sample,sqrtX_sample);
    sqrtX.write(sqrtX_sample);
  }

  while (!sqrtX.empty()) {

    // Get FIFO contents
    sqrtX_val = sqrtX.read();
    reference_x_val = reference_x.read();

    // Get reference value, scaled to output precision in cast
    expected_sqrtX = sqrt(reference_x_val.in);

    // Compare results for comparison against tolerance
    difference = sqrtX_val.out - expected_sqrtX;
    if (difference < 0) {
      difference *= -1;
    }

    if (difference > TOLERANCE) {
      fail |= 1;
      std::cout << "ERROR: sqrt output mismatch versus expected" << std::endl;
      std::cout << "Input " << reference_x_val.in << std::endl;
      std::cout << "Expected " << expected_sqrtX << std::endl;
      std::cout << "Actual  " << sqrtX_val.out << std::endl;
      std::cout << "Difference " << difference << std::endl;
      std::cout << "Tolerance " << TOLERANCE << std::endl << std::endl;
    } else {
      std::cout << "Input " << reference_x_val.in << std::endl;
      std::cout << "Expected " << expected_sqrtX << std::endl;
      std::cout << "Actual " << sqrtX_val.out << std::endl;
      std::cout << "Difference " << difference << std::endl;
      std::cout << "Tolerance " << TOLERANCE << std::endl << std::endl;
    }

  }

  if (fail == 0) {
    std::cout << "Test passed" << std::endl;
  } else {
    std::cout << "Test failed" << std::endl;
  }

  return(fail);
}
