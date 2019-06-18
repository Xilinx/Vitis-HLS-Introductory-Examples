#include <iostream>
#include "example.h"

int main()
{

  data_t t_a1, t_a2, t_a3, t_a4;
  data_t t_b1, t_b2, t_b3, t_b4;
  data_t t_sigma;

  // Test #1 ----------------------------------------------
  t_a1=3; t_a2=4; t_a3=1; t_a4=20;
  t_b1=7; t_b2=9; t_b3=0; t_b4=1023;
  
  example(t_a1, t_a2, t_a3, t_a4, t_b1, t_b2, t_b3, t_b4, t_sigma);

  std::cout << " t_sigma " << (int)t_sigma <<  std::endl;

  // Test #2 -----------------------------------------------
  t_a1=1; t_a2=0; t_a3=0; t_a4=0;
  t_b1=0; t_b2=0; t_b3=0; t_b4=0;
  
  example(t_a1, t_a2, t_a3, t_a4, t_b1, t_b2, t_b3, t_b4, t_sigma);

  std::cout << " t_sigma " << (int)t_sigma <<  std::endl;
  
}
