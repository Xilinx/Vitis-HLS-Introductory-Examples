/*
 * Copyright 2022 Xilinx, Inc.
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

#include "pointer_double.h"

data_t sub(data_t ptr[10], data_t size, data_t **flagPtr) {
  data_t x, i;

  x = 0;
  // Sum x if AND of local index and double-pointer index is true
  for (i = 0; i < size; ++i)
    if (**flagPtr & i)
      x += *(ptr + i);
  return x;
}

data_t pointer_double(data_t pos, data_t x, data_t *flag) {
  data_t array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  data_t *ptrFlag;
  data_t i;

  ptrFlag = flag;

  // Write x into index position pos
  if (pos >= 0 & pos < 10)
    *(array + pos) = x;

  // Pass same index (as pos) as pointer to another function
  return sub(array, 10, &ptrFlag);
}
