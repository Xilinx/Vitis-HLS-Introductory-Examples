#include "hls_fft.h"

// example configurable params
#define FFT_SSR 2

// SSR=2 data set: 1024 FFT
#if FFT_SSR == 2
#define FFT_CONFIG_WIDTH 16
#define FFT_NFFT_MAX 10
#elif FFT_SSR == 4 // SSR=4 data set: 64 FFT
#define FFT_CONFIG_WIDTH 8
#define FFT_NFFT_MAX 6
#else
#define FFT_CONFIG_WIDTH 16
#define FFT_NFFT_MAX 10
#endif


const char FFT_INPUT_WIDTH = 32;
const char FFT_OUTPUT_WIDTH = FFT_INPUT_WIDTH;
const int FFT_LENGTH = 1 << FFT_NFFT_MAX;

#include <complex>
using namespace std;

struct config1 : hls::ip_fft::ssr_params_t {
    static const unsigned super_sample_rate = FFT_SSR;

    static const bool use_native_float = true;

    OVERRIDE(systolicfft_inv) = false;

    static const unsigned config_width = FFT_CONFIG_WIDTH;

    static const unsigned max_nfft = FFT_NFFT_MAX;
};

typedef hls::ip_fft::config_t<config1> config_t;
typedef hls::ip_fft::status_t<config1> status_t;


typedef float data_in_t;
typedef float data_out_t;


typedef std::complex<data_in_t> cmpxDataIn;
typedef std::complex<data_out_t> cmpxDataOut;

void fft_top(bool direction, hls::stream<hls::vector<cmpxDataIn, FFT_SSR>> &in,
             hls::stream<hls::vector<cmpxDataOut, FFT_SSR>> &out, bool* ovflo);
