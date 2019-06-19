/*******************************************************************************
Vendor: Xilinx 
Associated Filename: window_fn_class.h
Purpose:Vivado HLSwindow_fn_float example project
Revision History: March 13, 2012 - initial release
                                                
*******************************************************************************
#-  (c) Copyright 2011-2019 Xilinx, Inc. All rights reserved.
#-
#-  This file contains confidential and proprietary information
#-  of Xilinx, Inc. and is protected under U.S. and
#-  international copyright and other intellectual property
#-  laws.
#-
#-  DISCLAIMER
#-  This disclaimer is not a license and does not grant any
#-  rights to the materials distributed herewith. Except as
#-  otherwise provided in a valid license issued to you by
#-  Xilinx, and to the maximum extent permitted by applicable
#-  law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
#-  WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
#-  AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
#-  BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
#-  INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
#-  (2) Xilinx shall not be liable (whether in contract or tort,
#-  including negligence, or under any other theory of
#-  liability) for any loss or damage of any kind or nature
#-  related to, arising under or in connection with these
#-  materials, including for any direct, or any indirect,
#-  special, incidental, or consequential loss or damage
#-  (including loss of data, profits, goodwill, or any type of
#-  loss or damage suffered as a result of any action brought
#-  by a third party) even if such damage or loss was
#-  reasonably foreseeable or Xilinx had been advised of the
#-  possibility of the same.
#-
#-  CRITICAL APPLICATIONS
#-  Xilinx products are not designed or intended to be fail-
#-  safe, or for use in any application requiring fail-safe
#-  performance, such as life-support or safety devices or
#-  systems, Class III medical devices, nuclear facilities,
#-  applications related to the deployment of airbags, or any
#-  other applications that could lead to death, personal
#-  injury, or severe property or environmental damage
#-  (individually and collectively, "Critical
#-  Applications"). Customer assumes the sole risk and
#-  liability of any use of Xilinx products in Critical
#-  Applications, subject only to applicable laws and
#-  regulations governing limitations on product liability.
#-
#-  THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
#-  PART OF THIS FILE AT ALL TIMES. 
#- ************************************************************************


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
(individually and collectively, "Critical Applications"). Customer assumes the 
sole risk and liability of any use of Xilinx products in Critical Applications, 
subject only to applicable laws and regulations governing limitations on product 
liability. 

THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT 
ALL TIMES.

*******************************************************************************/
#ifndef WINDOW_FN_CLASS_H_
#define WINDOW_FN_CLASS_H_

#include <cmath>
using namespace std;

// Create a unique namespace which can help avoid name clashes
namespace xhls_window_fn {

// Enumerated type for window type
typedef enum {
   RECT = 0, HANN, HAMMING, GAUSSIAN
} win_fn_t;

// The window_fn template class
// TI: input type - can be any floating or fixed point (ap_fixed<>) type
// TO: ouput type - same as TI
// TC: coefficient type - same as TI
// SZ: window size
// FT: function type - Hann, Hamming, etc; enumerated above
template<class TI, class TO, class TC, int SZ, win_fn_t FT = RECT>
class window_fn {
   const int win_len;
   const win_fn_t win_type;

public:
   window_fn(void) : win_len(SZ), win_type(FT) {};

   // The method that convolves indata w/ the window coeffs
   void apply(TO *outdata, TI *indata);

   // Get methods for polling class instance info - if not used, no hardware
   // will be created for them
   int get_len(void) { return win_len; };
   win_fn_t get_type(void) { return win_type; };
};

// This template function selects the coefficient equation specific to the
// window function type chosen at class object instantiation.
template<int SZ, win_fn_t FT>
double coef_calc(int i)
{
   double coef_val;

   switch(FT) {
   case RECT:
      coef_val = 1.0;
      break;
   case HANN:
      coef_val = 0.5 * (1.0 - cos(2.0 * M_PI * i / double(SZ)));
      break;
   case HAMMING:
      coef_val = 0.54 - 0.46 * cos(2.0 * M_PI * i / double(SZ));
      break;
   case GAUSSIAN:
      const double gaussian_sigma = 0.5;
      double x = (i - SZ / 2) / (gaussian_sigma * (SZ / 2));
      coef_val = exp(-0.5 * x * x);
      break;
   }
   return coef_val;
}

// This template function is used to initialize the contents of the 
// coefficient table.  Currently, in order for the table to be mapped to
// a ROM it must be defined at file (global) scope, i.e. it cannot be
// a class method (member function).
template<class TC, int SZ, win_fn_t FT>
void init_coef_tab(TC *coeff)
{
   for (int i = 0; i < SZ; i++) {
      coeff[i] = coef_calc<SZ,FT>(i);
   }
};

template<class TI, class TO, class TC, int SZ, win_fn_t FT>
void window_fn<TI,TO,TC,SZ,FT>::apply(TO *outdata, TI *indata)
{
   TC coeff_tab[SZ];
   // To ensure coeff_tab is implemented as a ROM on the FPGA, it must
   // be initialized by a separate function. No hardware is synthesized
   // for ROM initialization.
   init_coef_tab<TC,SZ,FT>(coeff_tab);
winfn_loop:
   for (unsigned i = 0; i < SZ; i++) {
//#pragma AP PIPELINE // implemented as TCL directive in this example
      outdata[i] = coeff_tab[i] * indata[i];
   }
}

}; // namespace xhls_window_fn

#endif // WINDOW_FN_CLASS_H_


