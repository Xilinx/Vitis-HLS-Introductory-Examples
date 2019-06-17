#include "filter_frame.h"

int main()
{

  // Filter coefficients
  coef_t H[TAP] = {1, 2, 2, 1};

  // Init output
  sum_t Y = 0;

  // Process frame 1
  data_t X[8] = {10, 4, 1, -7, 0, 0, 0, 0};
  filter(X, H, Y);
  cout << "frm1: " << Y << endl;

  // Process frame 2
  data_t A[8] = {2, 6, -8, 80, -79, 4, -8, -1};
  filter(A, H, Y);
  cout << "frm2: " << Y << endl;

  // Process frame 3
  data_t B[8] = {5, 1, 7, -2, 1, 20, -60, 100};
  filter(B, H, Y);
  cout << "frm3: " << Y << endl;

  // Process frame 4
  data_t C[8] = {2, 2, 2, 2, 2, 2, 2, 2};
  filter(C, H, Y);
  cout << "frm4: " << Y << endl;

  // Process frame 5
  data_t D[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  filter(D, H, Y);
  cout << "frm5: " << Y << endl;
}
