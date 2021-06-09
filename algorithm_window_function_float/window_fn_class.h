/*
 * Copyright 2021 Xilinx, Inc.
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
      outdata[i] = coeff_tab[i] * indata[i];
   }
}

}; // namespace xhls_window_fn

#endif // WINDOW_FN_CLASS_H_


