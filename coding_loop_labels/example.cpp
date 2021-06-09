/*
 * Copyright 2021 Xilinx, Inc.
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

/* This example shows the value of using loop labels as part of algorithm capture 
 * for Vivado HLS.
 * Both of the examples shown below are functionally equivalent. The difference is
 * in how directives can be applied to optimize the design. 
 * Directives are the main method to add tool commands for hardware level optimization
 * without having to incur the cost of a code modfication. In AutoESL, directives have
 * to be attached to a label. 
 */

/* example_no_label will show the for loop as for in the directive panel. 
 * At the time a directive is applied to this loop,Vivado HLSwill suggest and prompt
 * for a loop label name. Inserting the required loop label will change the original
 * source code.
 */
void example_no_label(int A[50], int B[50]) {
  int i;

  for(i = 0; i < 50; i++){
    B[i] = A[i] + 5;
  }
}

/* example_label will show the for loop as LOOP_1 in the directive panel. 
 * A directive can be applied to this label without any modification of the source
 * code.
 * At the time a directive is applied to this loop,Vivado HLSwill suggest and prompt
 * for a loop label name. 
 */
void example_label(int A[50], int B[50]) {
  int i;

 LOOP_1: for(i = 0; i < 50; i++){
    B[i] = A[i] + 5;
  }
}
