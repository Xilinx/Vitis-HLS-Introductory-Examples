#include <iostream>
#include <iomanip>
#include "fir.h"
using namespace std;

//-----------------------------------------
void run(data_t x, coef_t h[TAP], sum_t *y)
{
#pragma HLS INLINE off

  static systolic<data_t, coef_t, sum_t> s[TAP];

  data_t x_int[TAP];
  sum_t y_int[TAP];
  sum_t tmp = 0;

/* code */
connect:
  for (int i = 0; i < TAP; i++)
  {
    if (i == 0)
      s[0].exec(x, h[i], tmp, x_int[i], y_int[i]);
    else
      s[i].exec(x_int[i - 1], h[i], y_int[i - 1], x_int[i], y_int[i]);
    *y = y_int[TAP - 1];
  }
}

//-----------------------------------------------
void filter(data_t x[8], coef_t h[TAP], sum_t *y)
{
#pragma HLS INTERFACE ap_fifo depth=TAP port=y
#pragma HLS PIPELINE
#pragma HLS INTERFACE ap_fifo port=x
#pragma HLS ARRAY_PARTITION variable=h complete dim=1

unpack:
  for (int k = 0; k < 8; k++)
    run(x[k], h, y);
}

//------------------------------------------------
int main()
{

  // Filter coefficients
  coef_t H[TAP] = {1, 2, 2, 1};

  // Init output
  sum_t Y = 0;

  // Process frame X
  data_t X[8] = {10, 4, 1, 7, 0, 0, 0, 0};
  filter(X, H, &Y);
  cout << "X: " << Y << endl;

  // Process frame A
  data_t A[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  filter(A, H, &Y);
  cout << "A: " << Y << endl;

  // Process frame B
  data_t B[8] = {1, 1, 1, 1, 1, 1, 1, 1};
  filter(B, H, &Y);
  cout << "B: " << Y << endl;

  // Process frame C
  data_t C[8] = {2, 2, 2, 2, 2, 2, 2, 2};
  filter(C, H, &Y);
  cout << "C: " << Y << endl;

  // Process frame D
  data_t D[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  filter(D, H, &Y);
  cout << "D: " << Y << endl;
}
