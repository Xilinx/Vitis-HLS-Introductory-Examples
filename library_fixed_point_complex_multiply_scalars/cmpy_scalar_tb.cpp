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

#include "cmpy_scalar.h"

const bool VERBOSE_OUTPUT = false;

//Tolerance is calculated as 2.0/<expression below>, but atan introduces error too, so
//it is extended to 3.0.
double fn_get_phase_tolerance() {
  if (INPUT_WIDTH > OUTPUT_WIDTH) {
    return 3.0/(double)((ap_uint<INPUT_WIDTH+1>)1 << INPUT_WIDTH);
  } else {
    return 3.0/(double)((ap_uint<OUTPUT_WIDTH+1>)1 << OUTPUT_WIDTH);
  }
}
double fn_get_mag_tolerance() {
  if (INPUT_WIDTH > OUTPUT_WIDTH) {
    return 3.0/(double)((ap_uint<INPUT_WIDTH+1>)1 << (INPUT_WIDTH-INPUT_INTEGER_BITS));
  } else {
    return 3.0/(double)((ap_uint<OUTPUT_WIDTH+1>)1 << (OUTPUT_WIDTH-OUTPUT_INTEGER_BITS));
  }
}


//Uncomment the following line for more verbose output
//#define DEBUG

int main() {

  const int NUM_VALUES = 7; //per scalar input, this means 7^4 tests = 2041

  const t_input_scalar NEGMAX = -((ap_fixed<INPUT_WIDTH+1,INPUT_WIDTH+1>)1)<<(INPUT_INTEGER_BITS-1);
  const t_input_scalar POSMAX = ((ap_fixed<INPUT_WIDTH+1,INPUT_WIDTH+1>)1<<(INPUT_INTEGER_BITS-1)) - ((ap_fixed<INPUT_WIDTH,INPUT_INTEGER_BITS>)1 >> (INPUT_WIDTH-INPUT_INTEGER_BITS));
  //'arbitrary numbers NEGNUM and POSNUM are +/-1 -/+ 1ulp. This is intended to stress rounding.
  const t_input_scalar NEGNUM = -((ap_fixed<INPUT_WIDTH,INPUT_INTEGER_BITS>)1) + ((ap_fixed<INPUT_WIDTH,INPUT_INTEGER_BITS>)1 >>(INPUT_WIDTH-INPUT_INTEGER_BITS));
  const t_input_scalar POSNUM = ((ap_fixed<INPUT_WIDTH,INPUT_INTEGER_BITS>)1) - ((ap_fixed<INPUT_WIDTH,INPUT_INTEGER_BITS>)1>>(INPUT_WIDTH-INPUT_INTEGER_BITS));

  const t_input_scalar vals[NUM_VALUES] = {NEGMAX, NEGNUM, -1, 0, 1, POSNUM, POSMAX};
  //  const t_input_scalar vals[NUM_VALUES] = {-1, 0,  1};

  std::cout << "NEGMAX = " << NEGMAX << std::endl;
  std::cout << "POSMAX = " << POSMAX << std::endl;
  std::cout << "NEGNUM = " << NEGNUM << std::endl;
  std::cout << "POSNUM = " << POSNUM << std::endl;

  t_input_scalar ar, ai, br, bi;
  t_output_scalar pr, pi;

  int correct_tally = 0, wrong_tally = 0;
  int test_fail_for_sample;

  int count = 0;

  //polar validation
  double amag, bmag, pGotMag, pExpectedMag;
  double aphase, bphase, pGotPhase, pExpectedPhase;
  double phaseError, magError;
  double phaseTolerance = fn_get_phase_tolerance();
  double magTolerance = fn_get_mag_tolerance();

  //Start of testbench execution.
  for (int re_a_index = 0; re_a_index < NUM_VALUES; re_a_index++) {
    for (int re_b_index = 0; re_b_index < NUM_VALUES; re_b_index++) {
      for (int im_a_index = 0; im_a_index < NUM_VALUES; im_a_index++) {
        for (int im_b_index = 0; im_b_index < NUM_VALUES; im_b_index++) {
          count++;
          //create stimulus values
          ar = vals[re_a_index];
          ai = vals[im_a_index];
          br = vals[re_b_index];
          bi = vals[im_b_index];

          //execute function - both type variants
          cmpy_scalar_top(ar, ai, br, bi, pr, pi);

          if (VERBOSE_OUTPUT) {
            std::cout << "re_a_index = " << re_a_index << std::endl;
            std::cout << "re_b_index = " << re_b_index << std::endl;
            std::cout << "im_a_index = " << im_a_index << std::endl;
            std::cout << "im_b_index = " << im_b_index << std::endl;
            std::cout << "count = " << count << std::endl << std::endl;
          }

          test_fail_for_sample = 0; //ie. no tests passed yet.
          correct_tally++;

          //---------------------------
          //Validation...
          //By polar multiplication
          if ((ar == 0 && ai == 0) || (br == 0 && bi == 0)) {
            //skipping test
          } else {

            amag = sqrt(ar*ar + ai*ai);
            bmag = sqrt(br*br + bi*bi);
            aphase = atan2(ai,ar);
            bphase = atan2(bi,br);

            pGotMag = sqrt(pr*pr + pi*pi);
            pGotPhase = atan2(pi,pr);

            pExpectedPhase = aphase+bphase;
            //alias phase to one circle!
            while (pExpectedPhase > M_PI) {
              pExpectedPhase -= 2*M_PI;
            }
            while (pExpectedPhase < -M_PI) {
              pExpectedPhase += 2*M_PI;
            }
            pExpectedMag = amag*bmag;

            magError = abs(pGotMag-pExpectedMag);
            phaseError = pGotPhase - pExpectedPhase;

            while(phaseError > M_PI) {
              phaseError -= 2*M_PI;
            }
            while (phaseError < -M_PI) {
              phaseError += 2*M_PI;
            }

            if (magError > magTolerance) {
              std::cout << "Error : magnitude out of bounds. Got " << pGotMag << " Expected " << pExpectedMag << std::endl;
              wrong_tally++;
              test_fail_for_sample = 1;
            } else {
              correct_tally++;
            }
            if (phaseError > phaseTolerance ) {
              std::cout << "Error : phase out of bounds. Got " << pGotPhase << " Expected " << pExpectedPhase << std::endl;
              wrong_tally++;
              test_fail_for_sample = 1;
            } else {
              correct_tally++;
            }

            //...by identity and right angle rotations.
            if (ar == 0 && ai == 1) {
              //std::cout << "Testing rotation +90%" << std::endl;
              if (pr != -bi || pi != br) {
                test_fail_for_sample = 1;
                wrong_tally++;
              } else {
                correct_tally++;
              }
            }
            if (ar == 0 && ai == -1) {
              //std::cout << "Testing rotation -90%" << std::endl;
              if (pr != bi || pi != -br) {
                test_fail_for_sample = 1;
                wrong_tally++;
              } else {
                correct_tally++;
              }
            }
            if (ar == 1 && ai == 0) {
              //std::cout << "Testing identity" << std::endl;
              if (pr != br || pi != bi) {
                test_fail_for_sample = 1;
                wrong_tally++;
              } else {
                correct_tally++;
              }
            }
            if (br == 0 && bi == 1) {
              //std::cout << "Testing rotation +90%" << std::endl;
              if (pr != -ai || pi != ar) {
                test_fail_for_sample = 1;
                wrong_tally++;
              } else {
                correct_tally++;
              }
            }
            if (br == 0 && bi == -1) {
              //std::cout << "Testing rotation -90%" << std::endl;
              if (pr != ai || pi != -ar) {
                test_fail_for_sample = 1;
                wrong_tally++;
              } else {
                correct_tally++;
              }
            }
            if (br == 1 && bi == 0) {
              //std::cout << "Testing identity" << std::endl;
              //i.e. multiply by 1
              if (pr != ar || pi != ai) {
                test_fail_for_sample = 1;
                wrong_tally++;
              } else {
                correct_tally++;
              }
            }
            if (test_fail_for_sample > 0) {
              std::cout << "ERROR: unexpected result" << std::endl;
              std::cout << "ar " << ar << " ai " << ai << std::endl;
              std::cout << "br " << br << " bi " << bi << std::endl;
              std::cout << "pr " << pr << " pi " << pi << std::endl;
              std::cout << "amag = " << amag << " aphase = " << aphase << std::endl;
              std::cout << "bmag = " << bmag << " bphase = " << bphase << std::endl;
              std::cout << "pExpectedMag = " << pExpectedMag << " pExpectedPhase = " << pExpectedPhase << std::endl;
              std::cout << "pGotMag = " << pGotMag << " pGotPhase = " << pGotPhase << std::endl;
              std::cout << "Mag Tolerance = " << magTolerance << " Phase Tolerance = " << phaseTolerance << std::endl;
              std::cout << "Mag Error = " << magError << " Phase Error = " << phaseError << std::endl;
              std::cout << std::endl;
              exit(0);
            }
          }
        } //end of imag b loop
      } //end of imag a loop
    } //end of real b loop
  } //end of real a loop

  //report results
  std::cout << "Number of values tested = " << count << std::endl;
  std::cout << "Number of correct results = " << correct_tally << " (note that more than one test is applied to some values)" << std::endl;
  std::cout << "Number of incorrect results = " << wrong_tally << std::endl;
  if (correct_tally > 0 && wrong_tally == 0) {
    std::cout << "Test completed successfully" << std::endl << std::endl;
    return 0; //pass
  } else {
    return 1; //fail
  }

} //end of function main

