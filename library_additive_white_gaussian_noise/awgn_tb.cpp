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

//Example testbench for the HLS DSP library AWGN function

#include "math.h"
#include "ap_int.h"
#include "awgn.h"
#include <string>
#include <fstream>

//Parameters of test
const int NUM_BINS = 64;
const int LOG_BINS = 6;
const int NUM_SAMPLES = 1000;

int main() {

  const bool MORE_OUTPUT = false; // Enable verbose output
  int whichBin;
  int minBin = NUM_BINS/2;
  int maxBin = NUM_BINS/2;
  int ber = 0;
  int originalBit = 0;
  ap_int<40> bin[NUM_BINS];
  double berr = 0.0;
  double mean;
  double squareNoiseTally;
  double variance;
  double expected_variance;
  ap_int<OUTPUT_WIDTH>   softVal;
  ap_int<OUTPUT_WIDTH+2> receivedVal;
  int receivedBit;
  int undeterminedBit;

  //The following table is the expected error rate (as a fraction of the number of samples) for each snr possible
  //Note that this table is not used in the UUT, which is based on the probit function rather than the pdf of a normal distribution.
  //This gives the testbench immunity from that particular common-mode error.
  double normalPdfVsSnr[256] = {
    0.078649604, 0.077161546, 0.075684466, 0.074218592, 0.072764153, 0.071321372, 0.069890473, 0.068471676,
    0.067065198, 0.065671255, 0.064290057, 0.062921814, 0.061566729, 0.060225005, 0.058896839, 0.057582425,
    0.056281952, 0.054995605, 0.053723566, 0.052466009, 0.051223106, 0.049995023, 0.048781921, 0.047583956,
    0.046401276, 0.045234026, 0.044082345, 0.042946363, 0.041826207, 0.040721997, 0.039633844, 0.038561855,
    0.037506128, 0.036466757, 0.035443825, 0.034437411, 0.033447584, 0.032474407, 0.031517935, 0.030578216,
    0.029655288, 0.028749182, 0.027859923, 0.026987525, 0.026131995, 0.025293332, 0.024471527, 0.02366656,
    0.022878408, 0.022107033, 0.021352394, 0.02061444, 0.019893109, 0.019188335, 0.01850004, 0.01782814,
    0.017172542, 0.016533144, 0.015909838, 0.015302507, 0.014711024, 0.014135259, 0.013575069, 0.013030307,
    0.012500818, 0.011986439, 0.011487002, 0.011002329, 0.010532237, 0.010076538, 0.009635036, 0.009207529,
    0.008793811, 0.008393667, 0.008006881, 0.007633229, 0.007272484, 0.006924413, 0.006588781, 0.006265347,
    0.005953867, 0.005654095, 0.005365781, 0.005088673, 0.004822515, 0.004567051, 0.004322022, 0.004087169,
    0.003862232, 0.003646947, 0.003441055, 0.003244292, 0.003056397, 0.002877109, 0.002706168, 0.002543314,
    0.002388291, 0.002240842, 0.002100714, 0.001967655, 0.001841418, 0.001721755, 0.001608424, 0.001501186,
    0.001399805, 0.001304049, 0.001213691, 0.001128506, 0.001048275, 0.000972784, 0.000901823, 0.000835187,
    0.000772675, 0.000714093, 0.000659251, 0.000607964, 0.000560055, 0.000515348, 0.000473677, 0.000434879,
    0.000398796, 0.000365278, 0.000334179, 0.000305358, 0.000278681, 0.000254019, 0.000231247, 0.000210248,
    0.000190908, 0.000173119, 0.000156778, 0.000141787, 0.000128054, 0.00011549, 0.000104011, 9.35393E-05,
    8.39995E-05, 7.53216E-05, 6.74391E-05, 6.029E-05, 5.38159E-05, 4.7962E-05, 4.26773E-05, 3.79138E-05,
    3.36272E-05, 2.9776E-05, 2.63217E-05, 2.32285E-05, 2.04635E-05, 1.79962E-05, 1.57982E-05, 1.38437E-05,
    1.21089E-05, 1.05719E-05, 9.21259E-06, 8.0128E-06, 6.95578E-06, 6.02637E-06, 5.21076E-06, 4.49644E-06,
    3.87211E-06, 3.32754E-06, 2.85354E-06, 2.44183E-06, 2.085E-06, 1.77639E-06, 1.51009E-06, 1.28081E-06,
    1.08385E-06, 9.1504E-07, 7.70699E-07, 6.47573E-07, 5.42794E-07, 4.53848E-07, 3.78529E-07, 3.14908E-07,
    2.61307E-07, 2.16263E-07, 1.7851E-07, 1.46952E-07, 1.20643E-07, 9.87707E-08, 8.0637E-08, 6.56453E-08,
    5.32866E-08, 4.31281E-08, 3.48025E-08, 2.79996E-08, 2.24576E-08, 1.79567E-08, 1.43127E-08, 1.13718E-08,
    9.00601E-09, 7.10896E-09, 5.59283E-09, 4.38518E-09, 3.42652E-09, 2.66812E-09, 2.07026E-09, 1.60062E-09,
    1.23303E-09, 9.4636E-10, 7.23627E-10, 5.5122E-10, 4.18276E-10, 3.16159E-10, 2.38027E-10, 1.78485E-10,
    1.33293E-10, 9.91325E-11, 7.3418E-11, 5.41426E-11, 3.97559E-11, 2.90643E-11, 2.11538E-11, 1.53271E-11,
    1.10546E-11, 7.93618E-12, 5.67069E-12, 4.03261E-12, 2.85385E-12, 2.00976E-12, 1.40828E-12, 9.81835E-13,
    6.81019E-13, 4.69915E-13, 3.22541E-13, 2.20204E-13, 1.49522E-13, 1.00969E-13, 6.78021E-14, 4.52724E-14,
    3.00555E-14, 1.98371E-14, 1.30154E-14, 8.48846E-15, 5.50239E-15, 3.54476E-15, 2.26933E-15, 1.44359E-15,
    9.12396E-16, 5.72899E-16, 3.57344E-16, 2.21394E-16, 1.3623E-16, 8.3247E-17, 5.05132E-17, 3.04326E-17,
    1.82023E-17, 1.08074E-17, 6.36911E-18, 3.7252E-18, 2.16216E-18, 1.24521E-18, 7.11489E-19, 4.03286E-19
  };
  double expected_ber;
  double mean_thresh;
  double snr_scale;
  hls::stream< t_snr > InputData("SNR input value");
  hls::stream< ap_int<OUTPUT_WIDTH> > OutputData("AWGN output data");
  ap_int<OUTPUT_WIDTH> noise;
  ap_int<OUTPUT_WIDTH> errorThreshold = ((ap_int<OUTPUT_WIDTH>)1 << (OUTPUT_WIDTH-2));
  double noiseTotal = 0; //enough bits to count a billion samples.
  int snrInt;
  int numSamples;
  const t_snr MAX_SNR = 16.0;
  const double RECIP_SQRT_NUM_SAMPLES = 1.0/sqrt((double)NUM_SAMPLES);
  //MEAN_THRESH_CONST is 3stddevs for unscaled mean. It is later scaled by snr to create the mean thresh.
  const double MEAN_THRESH_CONST = 4*((ap_int<OUTPUT_WIDTH>)1<<(OUTPUT_WIDTH-2))/sqrt(2.0)*RECIP_SQRT_NUM_SAMPLES;


  for (t_snr snr = (t_snr)0.0; snr < MAX_SNR; snr+= (t_snr)0.1) {
    snrInt = (int)(snr*16);
    std::cout << "SNR = " << snr << std::endl;
    //Initialization
    ber = 0;
    squareNoiseTally = 0.0;
    undeterminedBit = 0; //A count of cases where noise + signal = exactly 0.
    for (int b = 0; b < NUM_BINS; b++) {
      bin[b] = 0;
    }
    noiseTotal = 0.0;

    //The loop length (numSamples) is such that the expected number of errors will
    //be ERRORS_DESIRED.
    //normalPdfVsSnr is the probability of an error at the given level of snr.
    //numSamples = ERRORS_DESIRED/normalPdfVsSnr[snrInt]+1;
    numSamples = 1000;
    for (int i = 0; i < numSamples; i++) {
      /////////////////////
      //Create the stimulus
      /////////////////////

      if (!InputData.full() ) {
        InputData << (t_snr)snr;

      ///////////////////
      //Execute the model
      ///////////////////
        awgn_top(InputData, OutputData);
      }
      /////////////////
      //Test the output
      /////////////////

      //read a noise sample from the infinite FIFO hls::stream
      if (!OutputData.empty()) {
        noise = OutputData.read();

        //create a signal sample (just toggle) to add the noise to.
        originalBit = 1-originalBit;
        if (originalBit == 1) {
          softVal = (ap_int<OUTPUT_WIDTH>)1 << (OUTPUT_WIDTH-2); //+  hence    01.000000...
        } else {
          softVal = -((ap_int<OUTPUT_WIDTH>)1 << (OUTPUT_WIDTH-2)); //- hence  11.000000...
        }

        //add noise to the signal - i.e. model a noisy channel
        receivedVal = noise+softVal;

        //interpret the resulting soft value
        if (receivedVal < 0) {
          receivedBit = 0;
        } else {
          receivedBit = 1;
        }
        if (receivedVal == 0) {
          undeterminedBit++;
        }

        if (receivedBit != originalBit) {
          ber++;
        }

        //Collect some stats
        noiseTotal += (double)noise;
        squareNoiseTally += noise*noise; //biased around zero
        whichBin = (noise + ((ap_int<OUTPUT_WIDTH>)1 << (OUTPUT_WIDTH-1))) >> (OUTPUT_WIDTH-LOG_BINS);
        if (whichBin < minBin) {
          minBin = whichBin;
        }
        if (whichBin > maxBin) {
          maxBin = whichBin;
        }
        bin[whichBin] = bin[whichBin] + 1;
      } // end if !OutputData.empty()
    } //end for i

    mean = noiseTotal/(double)numSamples;
    variance = (double)squareNoiseTally/(double)numSamples;

    //Note that by the nature of statistics a deterministic result cannot be guaranteed.
    //So an error reported does not necessarily mean the function is wrong. However,
    //errors reported here should be rare (< 0.1%)
    //mean tends to 0 as 1/sqrt(N) so chances of it being > 3std/sqrt(N) are <0.1%
    //and 5std/sqrt(N) > 1ppm
    snr_scale = pow(10,-((double)snrInt/(double)320.0));
    mean_thresh = snr_scale*MEAN_THRESH_CONST;
    if (abs(mean) > mean_thresh) {
      std::cout << "ERROR: mean expected to be less than " << mean_thresh << " for " << snr << " got " << mean << std::endl;
      return 1;
    }
    expected_variance = ((ap_int<OUTPUT_WIDTH*2>)1<<(2*OUTPUT_WIDTH-6))*2.0*snr_scale*snr_scale;

    if (variance / expected_variance > 1.2 || variance / expected_variance < 0.8) {
      std::cout << "ERROR: variance for snr = " << snr  << " Got " << variance << " expected " << expected_variance << std::endl;
      return 1;
    }

    if (MORE_OUTPUT) {
      for (int b = 0; b < NUM_BINS; b++) {
        std::cout << ", " << std::dec << bin[b] ;
      }
      //calc expected Bit Error Rate (BER), find measured BER
      berr = (double)ber/(double)numSamples;
      expected_ber = (double)numSamples * normalPdfVsSnr[snrInt];
      //Report out
      std::cout << "error threshold = " << std::hex << errorThreshold << std::endl;
      std::cout << std::endl;
      std::cout << "Num samples = " << numSamples << std::endl;
      std::cout << "Mean noise = " << mean << std::endl;
      std::cout << "Variance (based on mean of 0) = " << variance << std::endl;
      std::cout << "Std dev = " << sqrt(variance) << " vs signal amplitude of " << (1 << (OUTPUT_WIDTH-2)) << std::endl;
      std::cout << "Undetermined bits = " << undeterminedBit << std::endl;
      std::cout << "Got Bit errors = " << ber << std::endl;
      std::cout << "Expected Bit errors = " << std::dec << expected_ber << std::endl;
      std::cout << "Got Bit error rate = " << std::dec << berr << std::endl;
      std::cout << "Expected Bit error rate = " << std::dec << normalPdfVsSnr[snrInt] << std::endl;
    }
  } //end for snr

  return 0;

} //end of function main

