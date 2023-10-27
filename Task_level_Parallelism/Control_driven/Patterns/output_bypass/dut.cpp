
void pass(int tmp1[128], int b[128]) {
    int buff[127];
    for (int i = 0; i < 128; i++) {
        b[i] = tmp1[i];
    }
}

void split(int a[128], int tmp1[128], int tmp2[128]) {
    static int j;
    for (int i = 0; i < 128; i++) {
        j++;
        tmp1[i] = a[i];
        tmp2[i] = a[i];
    }
}

void dut(int a[128], int b[128], int tmp2[128]) {
#pragma HLS DATAFLOW

    int tmp1[128], tmp3[128];

    split(a, tmp1, tmp2);
    pass(tmp1, b);
}
