#include "test.h"

int main() {

    hls::stream<int, N> in;
    hls::stream<int, N/2> out1;
    hls::stream<int, N/2> out2;

  for (int i = 0; i < N; i++)
    in.write(i);
  
  odds_and_evens(in, out1, out2);


  int sum1 = 0, sum2 = 0;
  for (int i = 0; i < N/2; i++)
    sum1 += out1.read();
  for (int i = 0; i < N/2; i++)
    sum2 += out2.read();
  
  if ((sum1 != 30) && (sum2 != 30))
      return 1;
  
  return 0;
}

