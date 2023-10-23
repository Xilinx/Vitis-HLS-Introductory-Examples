#define DATA_SIZE 4096
#define BUFFER_SIZE 1024

void vadd(const unsigned int* in1, // Read-Only Vector 1
          const unsigned int* in2, // Read-Only Vector 2
          unsigned int* out_r,     // Output Result
          int size                 // Size in integer
          );
