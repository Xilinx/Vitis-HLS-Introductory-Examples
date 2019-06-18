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

#include <string>
#include <fstream>
#include "convolution_encoder.h"

int main (void){

  /*
    This example uses test vectors generated from the Xilinx LogiCORE Convolution Encoder v9.0 IP core
    to validate the function operation
  */

  hls::stream< ap_uint<1> > input_data("Input Data");
  hls::stream< ap_uint<OutputWidth> > output_data("Output Data"), expected_data("Expected Data");
  ap_uint<OutputWidth> output_symbol, expected_symbol;
  int output_count = 0;
  int error_count = 0;
  int data_mismatch = 0;
  int cycle =  0;

  // File I/O setup
  std::string infilename, outfilename, line;
  std::stringstream ss;
  std::ifstream infile, outfile;

  // The input and golden vector files are added to the testbench fileset in run_hls.tcl
  ss <<"din.dat";
  infilename = ss.str();
  ss.str("");
  ss <<"dout.dat";
  outfilename = ss.str();

  // Load file contents, push into queues
  std::cout << "Loading " << infilename.c_str() << " and " << outfilename.c_str() << std::endl;

  infile.open(infilename.c_str(), std::ifstream::in);
  if (infile.is_open())      {
    while ( getline (infile,line) ) {
      input_data << atoi(line.c_str());
    }
    infile.close();
  } else {
    std::cout << "ERROR: Unable to open file " << infilename.c_str() << std::endl;
    exit(1);
  }

  outfile.open(outfilename.c_str(), std::ifstream::in);
  if (outfile.is_open())      {
    while ( getline (outfile,line) ) {
      expected_data << atoi(line.c_str());
    }
    outfile.close();
  } else {
    std::cout << "ERROR: Unable to open file " << outfilename.c_str() << std::endl;
    exit(1);
  }

  if (input_data.empty() || expected_data.empty()) {
    std::cout << "ERROR: input_data or expected_data streams are empty despite files being read successfully" << std::endl;
    exit(1);
  }

  // Unload input queue into encoder, read encoder output as it becomes available
  while (!expected_data.empty()) {

    convolution_encoder_top(input_data, output_data);

    if (!output_data.empty()) {
      output_symbol = output_data.read();
      expected_symbol = expected_data.read();
      if (output_symbol != expected_symbol) {
        std::cout << cycle << " ERROR: Got " << output_symbol << " expected " << expected_symbol << std::endl;
        data_mismatch = 1;
        error_count++;
      }
      output_count++;
    }
    cycle++;
  }

  // Flush FIFO to avoid simulation warnings
  while (!output_data.empty()) {
    output_symbol = output_data.read();
  }

  while (!expected_data.empty()) {
    expected_symbol = expected_data.read();
  }

  std::cout << "--------------------------------------------------------------" << std::endl;
  std::cout << "Test Complete" << std::endl;
  std::cout << "--------------------------------------------------------------" << std::endl;
  std::cout << "Encoded " << output_count << " symbols " << std::endl;
  std::cout << "Comparison vs. expected had " << error_count << " mismatches" << std::endl;
  std::cout << "--------------------------------------------------------------" << std::endl;

  if (data_mismatch == 0) {
    std::cout << "Test passed" << std::endl;
  } else {
    std::cout << "Test failed" << std::endl;
  }

  return(data_mismatch);
}

