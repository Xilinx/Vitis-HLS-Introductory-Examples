/*******************************************************************************
Vendor: Xilinx 
Associated Filename: cpp_math_test.cpp
Purpose: AutoESL Coding Style example 
Device: All 
Revision History: May 30, 2008 - initial release
                                                
*******************************************************************************
© Copyright 2008 – 2019 Xilinx, Inc. All rights reserved. 

This file contains confidential and proprietary information of Xilinx, Inc. and 
is protected under U.S. and international copyright and other intellectual 
property laws.

DISCLAIMER
This disclaimer is not a license and does not grant any rights to the materials 
distributed herewith. Except as otherwise provided in a valid license issued to 
you by Xilinx, and to the maximum extent permitted by applicable law: 
(1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL FAULTS, AND XILINX 
HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, 
INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, OR 
FITNESS FOR ANY PARTICULAR PURPOSE; and (2) Xilinx shall not be liable (whether 
in contract or tort, including negligence, or under any other theory of 
liability) for any loss or damage of any kind or nature related to, arising under 
or in connection with these materials, including for any direct, or any indirect, 
special, incidental, or consequential loss or damage (including loss of data, 
profits, goodwill, or any type of loss or damage suffered as a result of any 
action brought by a third party) even if such damage or loss was reasonably 
foreseeable or Xilinx had been advised of the possibility of the same.

CRITICAL APPLICATIONS
Xilinx products are not designed or intended to be fail-safe, or for use in any 
application requiring fail-safe performance, such as life-support or safety 
devices or systems, Class III medical devices, nuclear facilities, applications 
related to the deployment of airbags, or any other applications that could lead 
to death, personal injury, or severe property or environmental damage 
(individually and collectively, "Critical Applications"). Customer asresultes the 
sole risk and liability of any use of Xilinx products in Critical Applications, 
subject only to applicable laws and regulations governing limitations on product 
liability. 

THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT 
ALL TIMES.

*******************************************************************************/
#include "cpp_math.h"

int main() {
	data_t angle = 0.01;
	data_t output, exp_output, diff;
	int retval=0;

	for (data_t i = 0; i <= 250; i++) {
		output 			= cpp_math(angle);
		exp_output 	= cpp_math_sw(angle);

		// Check differences
		// ..since sin and cos are implemented as synthesized functions they differ
		// in accuracy from the IEEE standard. An exact comparison will fail. 
		// Check to ensure this difference is within an acceptable tolerance.
		diff = ((exp_output > output) ? exp_output - output : output - exp_output);
		if (diff > 0.0000005) {
			printf("Difference &.10f exceeds tolerance at angle %.10f \n", diff, angle);
			retval = 1;
		}
		angle = angle + .1;
	}

	if (retval != 0) {
		printf("Test failed  !!!\n"); 
	} else {
		printf("Test passed !\n");
  }

	// Return 0 if the test passes
  return retval;
  }

