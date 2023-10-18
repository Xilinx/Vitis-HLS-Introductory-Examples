/**
 * Copyright (C) 2019-2022 Xilinx, Inc
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You may
 * not use this file except in compliance with the License. A copy of the
 * License is located at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#include <cstring>
#include <iostream>
#include <vector>

// XRT includes
#include "experimental/xrt_bo.h"
#include "experimental/xrt_device.h"
#include "experimental/xrt_kernel.h"

#include "types.h"

int main(int argc, char** argv) {
    unsigned int device_index = 0;
    std::string binaryFile = argv[1];

    std::cout << "Open the device" << device_index << std::endl;
    auto device = xrt::device(device_index);
    std::cout << "Opened the device" << device_index << std::endl;

    auto uuid = device.load_xclbin(binaryFile);

    size_t vector_size_bytes = sizeof(int) * totalNumWords;

    auto krnl = xrt::kernel(device, uuid, "diamond");

    std::cout << "Allocate Buffer in Global Memory\n";
    auto bufIn = xrt::bo(device, vector_size_bytes, krnl.group_id(0));
    auto bufOut = xrt::bo(device, vector_size_bytes, krnl.group_id(1));

    // Map the contents of the buffer object into host memory
    auto bufIn_map = bufIn.map<int*>();
    auto bufOut_map = bufOut.map<int*>();
    std::fill(bufIn_map, bufIn_map + totalNumWords, 0);
    std::fill(bufOut_map, bufOut_map + totalNumWords, 0);

    // Initialize the input data
    for (int i = 0; i < totalNumWords; i++)
        bufIn_map[i] = (uint32_t)i;
    std::cout << "The Test Data initialized" << std::endl;

    // Create the reference golden data for comparison
    int bufReference[totalNumWords];
    for (int i = 0; i < totalNumWords; ++i) {
        bufReference[i] = ((i * 3) + 25) + ((i * 3) * 2);
    }
    std::cout << "The Test Data created" << std::endl;

    std::cout << "Execution of the kernel\n";
    xrt::run run[3];

    for (int i = 0; i < 3; i++) {
        std::cout << "synchronize input buffer data to device global memory\n";
        bufIn.sync(XCL_BO_SYNC_BO_TO_DEVICE);
        run[i] = krnl(bufIn, bufOut, totalNumWords / 16);
        run[i].wait();
        std::cout << "Get the output data from the device" << std::endl;
        bufOut.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    }

#if debug
    for (int i = 0; i < totalNumWords; i++) {
        std::cout << "Referece  " << bufReference[i] << std::endl;
        std::cout << "Out  " << bufOut_map[i] << std::endl;
    }
#endif

    // Validate our results
    if (std::memcmp(bufOut_map, bufReference, totalNumWords))
        throw std::runtime_error("Value read back does not match reference");
    std::cout << "TEST PASSED\n";
    return 0;
}
