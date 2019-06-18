#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

#define ADDRBITS 14
#define NWORDS 1 << ADDRBITS

// Switch between regular and hw data types
#if 0
typedef int data_t;
typedef int addr_t;
#else
#include "ap_int.h"
#define DATAWIDTH 128
typedef ap_uint<DATAWIDTH> data_t;
typedef ap_uint<ADDRBITS> addr_t;
#endif

void resource_uram(bool wren, bool rden,
                   addr_t addrW, data_t datain,
                   addr_t AddrR, data_t *dataout);
