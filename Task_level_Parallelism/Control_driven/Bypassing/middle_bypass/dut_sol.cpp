

void bypass(int tmp1[128], int tmp2[128], int tmp3[128]) {

    for (int i = 0; i < 128; i++) {
        tmp3[i] = tmp1[i] + tmp2[i];
    }
}

void pass(int tmp2[128], int tmp4[128], int tmp1[128], int tmp5[128]) {
    //	static int j;

    int buff[127];
    for (int i = 0; i < 128; i++) {

        tmp4[i] = tmp2[i];
        tmp5[i] = tmp1[i];
    }
}

void double_pass(int a[128], int b[128], int tmp1[128], int tmp2[128]) {
    static int j;
    for (int i = 0; i < 128; i++) {
        j++;
        tmp1[i] = a[i];
        tmp2[i] = b[i];
    }
}

void dut(int a[128], int b[128], int tmp3[128]) {
#pragma HLS DATAFLOW

    int tmp1[128], tmp2[128], tmp4[128], tmp5[128];

    double_pass(a, b, tmp1, tmp2);
    pass(tmp2, tmp4, tmp1, tmp5);
    bypass(tmp5, tmp4, tmp3);
}
