#include "hls_task.h"
#define N 100
#include <iostream>

void sub_task1(hls::stream<int> &in, hls::stream<int> &out) {
  int c = in.read();
  out.write(c + 2);
}

void sub_task2(hls::stream<int> &in, hls::stream<int> &out) {
  int c = in.read();
  out.write(c - 1);
}

void task2(hls::stream<int> &in, hls::stream<int> &out, int n) {
  int c = in.read();
  std::cout << " Total value is " << c + 2 + n <<std::endl;
  out.write(c + 2 + n);
}

void test(hls::stream<int> &in, hls::stream<int> &out, int n) {
#pragma HLS STABLE variable=n
  HLS_TASK_STREAM<int> s1;
  HLS_TASK_STREAM<int> s2;
  HLS_TASK t(task2, s2, out, n);
  HLS_TASK t1(sub_task1, in, s1);
  HLS_TASK t2(sub_task2, s1, s2);
}

int main() {

  hls::stream<int> in;
  hls::stream<int> out;

  for (int i = 0; i < N; i++)
    in.write(i);
  test(in, out, N);


  int sum = 0;
  for (int i = 0; i < N; i++)
    sum += out.read();
  std::cout <<"sum i s" << sum <<std::endl;
  if (sum != 15250)
    return 1;
  return 0;
}
