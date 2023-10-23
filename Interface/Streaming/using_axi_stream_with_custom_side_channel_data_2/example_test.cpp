/*
 * Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
 * Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "example.h"

using namespace std;

int main() {

    hls::stream<packet> A, B;
    packet tmp1, tmp2;

    for (int j = 0; j < SIZE; j++) {

        tmp1.user = j;
        if (j == 99) {
            tmp1.last = 1;
        } else {
            tmp1.last = 0;
        }

        A.write(tmp1);
        example(A, B);
        B.read(tmp2);

        if (tmp1.get_user() + 5 != tmp2.get_user()) {
            cout << "ERROR: results mismatch" << endl;
            cout << "tmp1.user=" << tmp1.get_user();
            cout << " != ";
            cout << "tmp2.user=" << tmp2.get_user() << endl;
            return 1;
        }
    }

    cout << "Success: results match" << endl;
    return 0;
}
