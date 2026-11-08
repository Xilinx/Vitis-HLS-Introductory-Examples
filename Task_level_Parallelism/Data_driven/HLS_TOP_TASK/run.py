#
# Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
# Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import vitis
import os

cwd = os.getcwd()+'/'
# Initialize session
client = vitis.create_client()
client.set_workspace(path='./work_top_task')

# Delete the component if it already exists
if os.path.exists('./work_top_task/top_task'):
	client.delete_component(name='top_task')

# Create component. Create new config file in the component folder of the workspace
comp = client.create_hls_component(name='top_task', cfg_file = [cwd+'hls_config.cfg'], template = 'empty_hls_component')

# Run flow steps
comp = client.get_component(name='top_task')
comp.run(operation='C_SIMULATION')
comp.run(operation='SYNTHESIS')
comp.run(operation='CO_SIMULATION')
