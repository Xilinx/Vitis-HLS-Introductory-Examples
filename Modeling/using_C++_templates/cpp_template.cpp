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

#include "cpp_template.h"

// Tail recursive call
template <data_t N> struct fibon_s {
    template <typename T> static T fibon_f(T a, T b) {
        return fibon_s<N - 1>::fibon_f(b, (a + b));
    }
};

// Termination condition
template <> struct fibon_s<1> {
    template <typename T> static T fibon_f(T a, T b) { return b; }
};

void cpp_template(data_t a, data_t b, data_t& dout) {
    dout = fibon_s<FIB_N>::fibon_f(a, b);
}
