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

//Example testbench for the HLS DSP library NCO function
//Glossary
//PINC - Phase Increment
//POFF - Phase Offset
//SSR - Super Sample Rate, where more than one sample of sin and cos is output per call.

#include "math.h"
#include "ap_int.h"
#include "nco.h"
#include <string>
#include <fstream>

// Enable the following if output to stdout is desired.
const bool VERBOSE_OUTPUT = false;

// Maximum phase accumulator width
const int MAX_WIDTH = 48;

ap_uint<MAX_WIDTH> fn_get_tolerance(int accumWidth, int outputWidth) {
  if (accumWidth < outputWidth) {
    //Since the phase difference between samples is measured, this can be up to twice the
    //phase error in a sample. The phase error in a sample can be up to 2ulp because of
    //rounding errors in the output values (sin, cos) and in the arctan function, hence
    //tolerate up to 4 ulp.
    return (1 << 2);
  } else {
    //Here, the error is dominated by the output, since it is less precise, so scale the
    //tolerance accordingly.
    return (ap_uint<MAX_WIDTH>)1 << (accumWidth-outputWidth+2);
  }
}

int main() {

  const int PINC_INTERVAL = 100; //interval in samples between changes to stimulus value.
  const int poffInterval = 150;
  const int NUM_SAMPLES = 2000;

  ap_uint<ACCUM_WIDTH> pincSample, poffSample;
  hls::stream< ap_uint<ACCUM_WIDTH> > pincSampleQueue("PINC sample queue"), poffSampleQueue("POFF sample queue"); //a queue of values to stimulate the function with.
  //pinc and poff are the streams used for input to the core function
  hls::stream<ap_uint<ACCUM_WIDTH> > pinc("PINC");
  hls::stream<ap_uint<ACCUM_WIDTH> > poff("POFF");
  hls::stream< hls::t_nco_output_data<SUPER_SAMPLE_RATE, OUTPUT_WIDTH> > outputVal("NCO output data");
  hls::stream<ap_uint<OUTPUT_WIDTH> > sinValidationStream("Sine validation"); //similar to sinVal, but each entry is a single value rather than a vector <SuperSampleRate>
  hls::stream<ap_uint<OUTPUT_WIDTH> > cosValidationStream("Cosine validation");
  ap_int<OUTPUT_WIDTH> sinSingleSample,cosSingleSample;
  hls::t_nco_output_data<SUPER_SAMPLE_RATE, OUTPUT_WIDTH> outputSample;
  double measPhaseDouble;
  ap_uint<ACCUM_WIDTH> measPhase = 0, oldMeasPhase = 0;
  ap_uint<ACCUM_WIDTH> expectedPinc = 0, expectedPoff = 0, currentPhaseAcc = 0, expectedPhase, nextPinc= INIT_PINC, nextPoff = INIT_POFF;
  ap_uint<ACCUM_WIDTH> measError;
  int expectedPincChange = (SUPER_SAMPLE_RATE == 1 ? 0 : (SUPER_SAMPLE_RATE >> 1)+1);
  int expectedPoffChange = (SUPER_SAMPLE_RATE == 1 ? 0 : (SUPER_SAMPLE_RATE >> 1)+1);
  ap_uint<MAX_WIDTH> phaseTolerance = fn_get_tolerance(ACCUM_WIDTH, OUTPUT_WIDTH);
  int correct_tally = 0, wrong_tally = 0;
  int channel; //when SSR > 1 channel is a loop counter for the individual values within the output sample

  //Because the SSR always starts with an interrupt as though values had changed,
  //before the proxy has anything, expect 0's to start with on output.
  //Following that, the initial values should show through.
  //PINC stimulus values
  pincSampleQueue.write(200);
  pincSampleQueue.write(25001 % ((ap_uint<ACCUM_WIDTH+1>)1 << ACCUM_WIDTH));
  pincSampleQueue.write( ((ap_uint<ACCUM_WIDTH+1>)1 << ACCUM_WIDTH) -100); //negative steps
  pincSampleQueue.write( (ap_uint<ACCUM_WIDTH+1>)3 << (ACCUM_WIDTH-5)  );  //fixed angle, regardless of accum_width
  pincSampleQueue.write(15);

  //POFF stimulus values
  poffSampleQueue.write(50);
  poffSampleQueue.write(400);

  if (SUPER_SAMPLE_RATE == 1) {
    pincSample = INIT_PINC;
    poffSample = INIT_POFF;
  }

  for (int i = 0; i < NUM_SAMPLES; i++) {

    //Stimulate input streams
    if (SUPER_SAMPLE_RATE > 1) {
      //Note that when SSR > 1 the PINC stream FIFO is used to detect a new PINC value.
      //When there is a new PINC value the NCO will enter an interrupt period which itself cannot be interrupted.
      //Therefore, PINC values are not written to the NCO function on every call. They are only written when the value changes.
      if (i % poffInterval == poffInterval -1) {
        if (!poff.full() && !poffSampleQueue.empty()) {
          //write to dat files
          poffSampleQueue.read(poffSample);
          poff.write(poffSample);
          expectedPoffChange = i;
          nextPoff = poffSample;
        }
      }
      if (i % PINC_INTERVAL == PINC_INTERVAL -1 ) {
        if (!pinc.full() && !pincSampleQueue.empty()) {
          pincSampleQueue.read(pincSample);
          pinc.write(pincSample);
          expectedPincChange = i + (SUPER_SAMPLE_RATE >> 1)+1;
          nextPinc = pincSample;
          //Since a change to PINC will force an interrupt, any change to POFF during that interrupt will not be seen
          //until the end of that interrupt, so any pending change to POFF must be delayed until the expected PINC change.
          if (expectedPoffChange == i) {
            expectedPoffChange = expectedPincChange;
          }
        }
      }
    } else { //SSR ==1
      //With SSR == 1 there is no interrupt, so changes to PINC or POFF occur immediately (ignoring post-synthesis latency)
      if (i % poffInterval == poffInterval -1) {
        if (!poffSampleQueue.empty()) {
          //write to dat files
          poffSampleQueue.read(poffSample);
          nextPoff = poffSample;
          expectedPoff = nextPoff;
        }
      }
      if (i % PINC_INTERVAL == PINC_INTERVAL -1 ) {
        if (!pincSampleQueue.empty()) {
          pincSampleQueue.read(pincSample);
          nextPinc = pincSample;
          expectedPinc = nextPinc;
        }
      }
      if (!pinc.full()) {
        pinc.write(pincSample);
      }
      if (!poff.full()) {
        poff.write(poffSample);
      }
    }

    //Execution
    nco_top(pinc, poff, outputVal);

    //Read output into stream
    //For every one sinVal there will be SSR writes to the validation stream.
    while(!outputVal.empty()) {
      outputVal.read(outputSample);
      for (int j = 0; j<SUPER_SAMPLE_RATE; j++) {
        if (VERBOSE_OUTPUT) {
          std::cout << "cosSample = " << std::hex << outputSample.outputValue[j].real() << std::dec << std::endl;
          std::cout << "sinSample = " << std::hex << outputSample.outputValue[j].imag() << std::dec << std::endl;
        }
        cosSingleSample = outputSample.outputValue[j].real();
        cosValidationStream << cosSingleSample;
        sinSingleSample = outputSample.outputValue[j].imag();
        sinValidationStream << sinSingleSample;
      }
    }

    if (i == expectedPincChange) {
      expectedPinc = nextPinc;
      if (VERBOSE_OUTPUT) {
        std::cout << "Change of expectedPinc = " << expectedPinc << std::endl;
      }
    }
    if (i == expectedPoffChange) {
      expectedPoff = nextPoff;
      if (VERBOSE_OUTPUT) {
        std::cout << "Change of expectedPoff = " << expectedPoff << std::endl;
      }
    }

    //Validate function output
    channel = 0;
    while(!sinValidationStream.empty()) { //strictly speaking this should test cosValidationStream too, but they should be in lock-step. A single stream of a struct holding both values could also be used.
      sinSingleSample = sinValidationStream.read();
      cosSingleSample = cosValidationStream.read();
      //arctan returns radians, but PINC and POFF are in terms of 2*pi/2^(AccumWidth), so it is necessary to scale.
      measPhaseDouble = atan2(sinSingleSample,cosSingleSample)*((ap_uint<ACCUM_WIDTH+1>)1 << ACCUM_WIDTH)/(2*M_PI);
      measPhase = (ap_uint<ACCUM_WIDTH>)(measPhaseDouble+0.5); //simple rounding
      if (VERBOSE_OUTPUT) {
        std::cout << "measPhaseDouble = " << measPhaseDouble << std::endl;
        std::cout << "sample = " << i << " channel = " << channel << std::endl;
        std::cout << "sin = " << sinSingleSample << std::endl;
        std::cout << "cos = " << cosSingleSample << std::endl;
        std::cout << "measPhase = " << measPhase << " diff = " << (measPhase-oldMeasPhase) << std::endl;
        oldMeasPhase = measPhase;
        channel++;
      }

      //Predict what the measured phase ought to be.
      expectedPhase = (currentPhaseAcc + expectedPinc + expectedPoff);

      measError = (ap_uint<ACCUM_WIDTH>)(abs(expectedPhase - measPhase));

      //trap the case where the error is approx 359 degrees.
      if (measError > (ap_uint<ACCUM_WIDTH+1>)1 << (ACCUM_WIDTH-1)) { //i.e. >180 degress
        measError = ((ap_uint<ACCUM_WIDTH+1>)1 << ACCUM_WIDTH) - measError;
      }
      if (VERBOSE_OUTPUT) {
        std::cout << "expectedPhase = " << expectedPhase << " currentPhaseAcc = " << currentPhaseAcc << " expectedPinc = " << expectedPinc << " expectedPoff = " << expectedPoff << std::endl;
        std::cout << "measError = " << measError << std::endl;
      }
      if ( measError < phaseTolerance )  {
        if (VERBOSE_OUTPUT) {
          std::cout << "Phase as expected. Phase error = " << measError << " Phase Tolerance = " << phaseTolerance << std::endl;
        }
        //reset estimate of nco's phase accumulator on each tick rather than allow errors to accumulate (drift)
        currentPhaseAcc = (ap_uint<ACCUM_WIDTH>)(measPhase - expectedPoff);
        correct_tally++;
      } else {
        std::cout << "Error: output phase is not as expected." << std::endl;
        std::cout << "Expected = " << expectedPhase << " got = " << measPhase << std::endl;
        std::cout << "ExpectedPinc = " << expectedPinc << " ExpectedPoff = " << expectedPoff << std::endl;
        std::cout << "i.e. measError = " << measError << std::endl;
        std::cout << "Sample = " << i << std::endl;
        std::cout << "sin = " << sinSingleSample << std::endl;
        std::cout << "cos = " << cosSingleSample << std::endl;
        std::cout << "measPhase = " << measPhase << std::endl;
        std::cout << "currentPhaseAcc = " << currentPhaseAcc << std::endl;
        std::cout << "nextPinc = " << nextPinc << std::endl;
        std::cout << "nextPoff = " << nextPoff << std::endl;
        wrong_tally++;
      } //end of if measErr < phaseTolerance
    } //end of while (validation)
    if (VERBOSE_OUTPUT) {
      std::cout << std::endl;
    }
  } //end of for i (sample loop)

  //Report out results
  if (wrong_tally == 0 && correct_tally == NUM_SAMPLES*SUPER_SAMPLE_RATE) {
    std::cout << "Test completed successfully. " << correct_tally << " samples tested" << std::endl;
    return 0;
  } else {
    std::cout << "Test FAILURE!" << std::endl;
    std::cout << "Number of samples within tolerance = " << correct_tally << std::endl;
    std::cout << "Number of samples out of tolerance = " << wrong_tally << std::endl;
    return 1;
  }

} //end of function main

