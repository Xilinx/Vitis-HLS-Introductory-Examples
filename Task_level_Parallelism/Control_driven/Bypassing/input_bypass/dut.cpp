

void add_kernel(int tmp1[128], int tmp2[128], int tmp3[128]) {

    int buff[127];
    for (int i = 0; i < 128; i++) {
        tmp3[i] = tmp1[i] + tmp2[i];
    }
}

void double_pass(int b[128], int tmp2[128], int tmp1[128], int tmp4[128]) {

    int buff[127];
    for (int i = 0; i < 128; i++) {
        tmp2[i] = b[i];
        tmp4[i] = tmp1[i];
    }
}

void pass(int a[128], int tmp1[128]) {
    for (int i = 0; i < 128; i++) {
        tmp1[i] = a[i];
    }
}

void dut(int a[128], int b[128], int tmp3[128]) {
#pragma HLS DATAFLOW

    int tmp1[128], tmp2[128], tmp4[128];

    pass(a, tmp1);
    double_pass(b, tmp2, tmp1, tmp4);
    add_kernel(tmp4, tmp2, tmp3);
}
