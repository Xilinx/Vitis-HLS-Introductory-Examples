#ifndef ADDER_H
#define ADDER_H

// Template function declaration
template<int PIPE, int FACTOR>
void adder(int a[10], int b[10], int z[10]);

// Top-level function declaration
void top(int a[10], int b[10], int c[10], int z[10]);

#endif // ADDER_H