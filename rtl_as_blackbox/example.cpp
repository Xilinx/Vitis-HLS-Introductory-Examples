#include "example.h"
//--------------------------------------------------------
void rtl_model(data_t  a1, data_t  a2, data_t  a3, data_t  a4,
	       data_t  b1, data_t  b2, data_t  b3, data_t  b4,
	       data_t &z1, data_t &z2, data_t &z3, data_t &z4);
//--------------------------------------------------------
void example(data_t  a1, data_t  a2, data_t  a3, data_t  a4,
	     data_t  b1, data_t  b2, data_t  b3, data_t  b4,
	     data_t &sigma) {

  data_t tmp1, tmp2,tmp3, tmp4;

  rtl_model(a1, a2, a3, a4, b1, b2, b3, b4, tmp1, tmp2, tmp3, tmp4);
  sigma = tmp1 + tmp2 + tmp3 + tmp4;
}
//--------------------------------------------------------
