#include "ap_axi_sdata.h"
#include "hls_stream.h"
#include <complex>

#define SIZE 5

typedef hls::axis<std::complex<short int>, 0, 0, 0> data_t;
typedef hls::stream<data_t> mystream;

extern void example(mystream& A, mystream& B);
