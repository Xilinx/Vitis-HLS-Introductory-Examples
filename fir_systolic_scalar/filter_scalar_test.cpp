#include "filter_scalar.h"

int main()
{
  data_t X[20] = {10, -4, 1, 7, 1, 1, 0, 0, 1, 1, 1, -6, -2, -1, 2, 2, 0, 0, 0, 0};
  coef_t H[TAP] = {1, 2, 2, 1};
  sum_t Y = 0;

  for (int i = 0; i < 20; i++)
  {
    filter(X[i], H, Y);
    cout << "Iter:" << setw(3) << right << i << " with x" << setw(3) << right
    << X[i] << " ###:" << setw(3) << right << Y << endl;
  }
}