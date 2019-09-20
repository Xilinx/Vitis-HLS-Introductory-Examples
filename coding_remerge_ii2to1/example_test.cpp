#include "example.h"

void example(stream<int> &in, stream<int> &out);

int main()
{

  stream<int> in("in_stream");
  stream<int> out("out_stream");

  int N = 50;
  for (int i = 0; i < N; i++)
    in << i;

  example(in, out);

  for (int p = 0; p < N; p++)
  {
    int var;
    out.read(var);
    if (var != p)
    {
      std::cout << "Test pattern failed" << std::endl;
      return 1;
    }
  }
  return 0;
}