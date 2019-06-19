#include <iostream>
#include <stdlib.h>
#include "example.h"

void example(strm_t &in1, strm_t &in2, strm_t &out);

int main()
{
  // Declare streams
  strm_t in1, in2, out;

  // Write data into a and b
  for (int i = 0; i < 10; i++)
  {
    int a = rand() % 10;
    int b = rand() % 10;
    in1.write(a);
    in2.write(b);
  }

  example(in1, in2, out);
  
  int sum = out.read();
  
  std::cout << "sum = " << sum << std::endl;
  
  return 0;
}
