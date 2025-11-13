template<int TripCount>
void dot_product(const int* a, const int* b, int& output) {
    output = 0;
    DP: for (int i = 0; i < TripCount; ++i) {
        //#pragma HLS if (TripCount > 20) pipeline II = 1
        //#pragma HLS if (TripCount <=20) unroll	
        output += a[i] * b[i];
    }
}


void top(const int* a, const int* b, int& output) {
#pragma HLS INTERFACE m_axi port=a depth=10 bundle=gmem
#pragma HLS INTERFACE m_axi port=b depth=10 bundle=gmem
#pragma HLS INTERFACE m_axi port=output depth=10 bundle=gmem

    dot_product<10>(a, b, output); // Instantiate with TripCount = 30
}

template<int TripCount>
int run_test() {
    int a[TripCount], b[TripCount], result = 0, expected = 0;

    for (int i = 0; i < TripCount; ++i) {
        a[i] = i + 1;
        b[i] = (i + 1) * 2;
        expected += a[i] * b[i];
    }

    top(a, b, result);
    if (result == expected)
	return 0;
    else
	return 1;
}

int main() {
	return run_test<10>();
}

