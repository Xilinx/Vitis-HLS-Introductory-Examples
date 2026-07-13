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
#
# The current directory basename is used as the component name, source file
# stem, testbench file stem, and top-level HLS function name.
import vitis
import os

cwd = os.getcwd()
component_name = 'unaligned_burst_rgb'
workspace_path = './workspace/'
syn_file = component_name + '.cpp'
tb_file = component_name + '_test.cpp'
top_function = component_name

print("(i) creating Vitis client, workspace: " + workspace_path)
client = vitis.create_client(workspace=workspace_path)

# Delete the component if it already exists
if os.path.exists(workspace_path+component_name):
    client.delete_component(name=component_name)

print("(i) creating HLS component: " + component_name)
comp = client.create_hls_component(name=component_name, template='empty_hls_component')

cfg_file = client.get_config_file(path=workspace_path+component_name+'/'+'hls_config.cfg')
cfg_file.set_value(key='part', value='xcvc1902-vsva2197-2MP-e-S')
cfg_file.set_value(section='hls', key='syn.file', value=cwd+'/'+syn_file)
cfg_file.set_values(section='hls', key='tb.file', values=[cwd+'/'+tb_file])
cfg_file.set_value(section='hls', key='syn.top', value=top_function)
cfg_file.set_value(section='hls', key='clock', value='2.5ns')
cfg_file.set_value(section='hls', key='flow_target', value='vivado')
cfg_file.set_value(section='hls', key='package.output.syn', value='0')
cfg_file.set_value(section='hls', key='csim.code_analyzer', value='0')

comp = client.get_component(name=component_name)
comp.run(operation='C_SIMULATION')
comp.run(operation='SYNTHESIS')
comp.run(operation='CO_SIMULATION')
