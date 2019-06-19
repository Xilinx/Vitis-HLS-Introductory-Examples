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

#include "viterbi_decoder.h"

// Convolution encoder
template<int OutputRate> void conv_enc(hls::stream< ap_uint<1> > &ConvEncInputData,
                                       hls::stream< ap_uint<OutputRate> > &ConvEncOutputData) {

  ap_uint<1> encoder_in;
  ap_uint<OutputRate> encoded_data;
  static const int MAX_CODES = 7;
  ap_uint<ConstraintLength> convolution_codes[MAX_CODES];
  convolution_codes[0] = ConvolutionCode0;
  convolution_codes[1] = ConvolutionCode1;
  convolution_codes[2] = ConvolutionCode2;
  convolution_codes[3] = ConvolutionCode3;
  convolution_codes[4] = ConvolutionCode4;
  convolution_codes[5] = ConvolutionCode5;
  convolution_codes[6] = ConvolutionCode6;
  static ap_uint<ConstraintLength-1> constraint_register;

  encoder_in = ConvEncInputData.read();

  // Modulo-2 additions
  for (int i=0; i<OutputRate; i++) {
    ap_uint<ConstraintLength> data = (constraint_register,encoder_in); // temporary variable to allow use of .reverse()
    encoded_data[i] = (data.reverse() & (convolution_codes[i])).xor_reduce();
  }

  ConvEncOutputData << encoded_data;

  // Update constraint register
  constraint_register = (constraint_register,encoder_in);
}

// Uniform integer in range(inclusive) [1,2147483646]
// Never seed with zero
int ran0(int& idum) {
  const int IA=16807;
  const int IM=2147483647;
  const int IQ=127773;
  const int IR=2836;

  int k=idum/IQ;
  idum=IA*(idum-k*IQ)-IR*k;
  if (idum<0) idum+=IM;

  return idum;
}

int main (void){

  const int INPUT_SYMBOLS = 10000;

  hls::stream< ap_uint<1> > bits_in("Bits In");
  hls::stream< hls::viterbi_decoder_input<OutputRate,InputDataWidth,HasEraseInput> > input_data("Input Data");
  hls::stream< ap_uint<OutputRate> > encoded_data("Encoded Data");
  hls::stream< ap_uint<1> > output_data("Output Data"), expected_data("Expected Data");

  hls::viterbi_decoder_input<OutputRate,InputDataWidth,HasEraseInput> input, encoded;
  ap_uint<1> output_bit, expected_bit;
  int output_count = 0;
  int inserted_error_count = 0;
  int data_mismatch = 0;
  int decode_error_count = 0;

  int sym_seed = 13;

  for (int i=0; i<INPUT_SYMBOLS; i++) {
    ap_uint<1> bit;

    bit = (sym_seed % 2);

    bits_in << bit;
    expected_data << bit;
    sym_seed = ran0(sym_seed);
  }

  while(!bits_in.empty()) {
    conv_enc(bits_in, encoded_data);

    ap_uint<OutputRate> enc_data_hard = encoded_data.read();
    ap_uint<OutputRate*InputDataWidth> enc_data = 0; // accomodates hard and soft data

    if (!SoftData) {
      enc_data = enc_data_hard;
    } else {
      if (SoftDataFormat == 0) {
	// SIGNED_MAGNITUDE
	for (int input = 0; input < OutputRate; input++) {
	  for (int bit = 0; bit < InputDataWidth; bit++) {
	    if (bit == InputDataWidth-1 && enc_data_hard[input] == 0) {
	      enc_data[(InputDataWidth*input) + bit] = 0; // -Fullscale + 1 LSB
	    } else {
	      enc_data[(InputDataWidth*input) + bit] = enc_data_hard; // +Fullscale
	    }
	  }
	}
      } else if (SoftDataFormat == 1) {
	// OFFSET_BINARY
	for (int input = 0; input < OutputRate; input++) {
	  for (int bit = 0; bit < InputDataWidth; bit++) {
	    if (enc_data_hard[input] == 0) {
	      enc_data[(InputDataWidth*input) + bit] = 0; // -Fullscale
	    } else {
	      enc_data[(InputDataWidth*input) + bit] = 1; // +Fullscale
	    }
	  }
	}
      } else {
	std::cout << "ERROR: bad soft data format" << std::endl;
	exit(1);
      }
    }


    if(sym_seed % 999 == 0) {
      // Introduce occasional errors to verify error-correcting capability
      ap_uint<OutputRate*InputDataWidth> sym = enc_data;
      ap_uint<OutputRate*InputDataWidth> err_sym;
      for (int bit = 0; bit < OutputRate*InputDataWidth; bit++) {
	if (bit % 3 == 0) {
	  err_sym[bit] = ~sym[bit];
	}
      }
      input.data = err_sym;
      input_data << input;
      std::cout << "Inserting error: symbol was " << sym << " now " << err_sym << std::endl;
      inserted_error_count++;
    } else {
      input.data = enc_data;
      input_data << input;
    }
    sym_seed = ran0(sym_seed);
  }

  while(!input_data.empty()) { // for local conv enc and random data

    viterbi_decoder_top(input_data, output_data);

    if(!output_data.empty()) { // blocking read
      output_bit = output_data.read();
      expected_bit = expected_data.read();
      if (output_bit != expected_bit) {
	std::cout << "ERROR: (" << output_count << ") " << "output_bit " << output_bit << " doesn't match expected_bit " << expected_bit << std::endl;
	data_mismatch = 1;
	decode_error_count++;
      }
      output_count++;
    }

  }

  // Flush FIFOs to avoid simulation warnings
  while (!output_data.empty()) {
    output_bit = output_data.read();
  }

  while (!expected_data.empty()) {
    expected_bit = expected_data.read();
  }

  std::cout << "--------------------------------------------------------------" << std::endl;
  std::cout << "Test Complete" << std::endl;
  std::cout << "--------------------------------------------------------------" << std::endl;
  std::cout << "Inserted " << inserted_error_count << " errors " << std::endl;
  std::cout << "--------------------------------------------------------------" << std::endl;
  std::cout << "Decoded " << output_count << " bits " << std::endl;
  std::cout << "Failed to correct " << decode_error_count << " bits " << std::endl;
  std::cout << "--------------------------------------------------------------" << std::endl;

  if (data_mismatch == 0) {
    std::cout << "Test passed" << std::endl;
  } else {
    std::cout << "Test failed" << std::endl;
  }

  return(data_mismatch);
}

