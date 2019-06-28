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

#include "atan2.h"
#include <cmath>

int main (void){

  const int SAMPLES = 8;
  const int INTEGER_BITS = 3;
  const ap_fixed<OutputWidth,3,AP_TRN_ZERO,AP_WRAP,0> TOLERANCE = pow(2,-(OutputWidth-(INTEGER_BITS+1)));// Equivalent to setting second LSB

  hls::atan2_input<InputWidth>::cartesian x_val;
  hls::atan2_input<InputWidth>::cartesian reference_x_val;
  hls::atan2_output<OutputWidth>::phase atanX_val;
  ap_fixed<OutputWidth,3,AP_TRN_ZERO,AP_WRAP,0> expected_atanX;
  ap_fixed<OutputWidth,3,AP_TRN_ZERO,AP_WRAP,0> difference;

  hls::stream< hls::atan2_input<InputWidth>::cartesian > x("Input Data: x");
  hls::stream< hls::atan2_input<InputWidth>::cartesian > reference_x("Reference Input Data: x");
  hls::stream< hls::atan2_output<OutputWidth>::phase > atanX("Output Data: atan2(x)");

  hls::atan2_input<InputWidth>::cartesian x_sample;
  hls::atan2_output<OutputWidth>::phase atanX_sample;

  int fail = 0;

  // Test with a point from each quadrant, and one on each axis
  // Store the results in queues
  for (int sample=1; sample<=SAMPLES; sample++) {
    const float re = 0.5;
    const float im = 0.5;
    float scale_re = 1;
    float scale_im = 1;
    if (sample <= SAMPLES/4) {
      if (sample % 2 == 0) {
        scale_re = 1;
        scale_im = 1;
      } else {
        scale_re = 1;
        scale_im = 0;
      }
    } else if (sample > SAMPLES/4 && sample <= SAMPLES/2) {
      if (sample % 2 == 0) {
        scale_re = -1;
        scale_im = 1;
      } else {
        scale_re = 0;
        scale_im = 1;
      }
    } else if (sample > SAMPLES/2 && sample <= 3*SAMPLES/4) {
      if (sample % 2 == 0) {
        scale_re = -1;
        scale_im = -1;
      } else {
        scale_re = -1;
        scale_im = 0;
      }
    } else {
      if (sample % 2 == 0) {
        scale_re = 1;
        scale_im = -1;
      } else {
        scale_re = 0;
        scale_im = -1;
      }
    }
    x_val.cartesian.real(re*scale_re);
    x_val.cartesian.imag(im*scale_im);
    x << x_val;
    reference_x << x_val;
  }

  // Call wrapped arctan
  while (!x.empty()) {
    x.read(x_sample);
    atan2_top(x_sample,atanX_sample);
    atanX.write(atanX_sample);
  }

  while (!atanX.empty()) {

    // Get FIFO contents
    atanX_val = atanX.read();
    reference_x_val = reference_x.read();

    // Get reference value, scaled to output precision in cast
    expected_atanX = atan2(reference_x_val.cartesian.imag(),reference_x_val.cartesian.real());

    // Take local copy of function output
    ap_fixed<OutputWidth,3,AP_TRN_ZERO,AP_WRAP,0> atanX_val_tmp = atanX_val.phase;

    // Compensate for implementation differences
    if (reference_x_val.cartesian.imag() == 0 && reference_x_val.cartesian.real() < 0) {
      if (-atanX_val_tmp == expected_atanX) {
        atanX_val_tmp *= -1;
        std::cout << "Ignoring sign difference between -pi and pi when on Y axis" << std::endl;
      }
    }

    // Compare results for comparison against tolerance
    difference = atanX_val_tmp - expected_atanX;
    if (difference < 0) {
      difference *= -1;
    }

    if (difference > TOLERANCE) {
      fail |= 1;
      std::cout << "ERROR: atan2 output mismatch versus expected" << std::endl;
      std::cout << "X axis input " << reference_x_val.cartesian.real() << std::endl;
      std::cout << "Y axis input " << reference_x_val.cartesian.imag() << std::endl;
      std::cout << "Expected " << expected_atanX << std::endl;
      std::cout << "Actual  " << atanX_val.phase << std::endl;
      std::cout << "Difference " << difference << std::endl;
      std::cout << "Tolerance " << TOLERANCE << std::endl << std::endl;
    } else {
      std::cout << "X axis input " << reference_x_val.cartesian.real() << std::endl;
      std::cout << "Y axis input " << reference_x_val.cartesian.imag() << std::endl;
      std::cout << "Expected " << expected_atanX << std::endl;
      std::cout << "Actual " << atanX_val.phase << std::endl;
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

