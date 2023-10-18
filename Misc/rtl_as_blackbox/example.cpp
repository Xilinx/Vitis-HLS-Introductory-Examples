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
//--------------------------------------------------------
void rtl_model(data_t a1, data_t a2, data_t a3, data_t a4, data_t b1, data_t b2,
               data_t b3, data_t b4, data_t& z1, data_t& z2, data_t& z3,
               data_t& z4);
//--------------------------------------------------------
void example(data_t a1, data_t a2, data_t a3, data_t a4, data_t b1, data_t b2,
             data_t b3, data_t b4, data_t& sigma) {

    data_t tmp1, tmp2, tmp3, tmp4;

    rtl_model(a1, a2, a3, a4, b1, b2, b3, b4, tmp1, tmp2, tmp3, tmp4);
    sigma = tmp1 + tmp2 + tmp3 + tmp4;
}
//--------------------------------------------------------
