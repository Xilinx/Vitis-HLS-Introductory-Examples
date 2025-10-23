
import vitis
import os

cwd = os.getcwd()+'/'

# Initialize session
client = vitis.create_client()
client.set_workspace(path='./work_using_free_running_pipeline')

# Delete the component if it already exists
if os.path.exists('./work_using_free_running_pipeline/using_free_running_pipeline'):
	client.delete_component(name='using_free_running_pipeline')

# Create component. Create new config file in the component folder of the workspace
comp = client.create_hls_component(name='using_free_running_pipeline', cfg_file = ['hls_config.cfg'], template = 'empty_hls_component')

# Get handle of config file, then programmatically set desired options
cfg_file = client.get_config_file(path = './work_using_free_running_pipeline/using_free_running_pipeline/hls_config.cfg')
cfg_file.set_value (                 key = 'part',                  value = 'xcvu9p-flga2104-2-i') 
cfg_file.set_value (section = 'hls', key = 'syn.file',              value = cwd+'free_pipe_mult.cpp')
cfg_file.set_values(section = 'hls', key = 'tb.file',               values = [cwd+'free_pipe_mult_tb.cpp', cwd+'result.golden.dat'])
cfg_file.set_value (section = 'hls', key = 'syn.top',               value = 'free_pipe_mult')
cfg_file.set_value (section = 'hls', key = 'clock',                 value = '4') # 250MHz
cfg_file.set_value (section = 'hls', key = 'flow_target',           value = 'vivado')
cfg_file.set_value (section = 'hls', key = 'package.output.syn',    value = '0')
cfg_file.set_value (section = 'hls', key = 'package.output.format', value = 'rtl')
cfg_file.set_value (section = 'hls', key = 'csim.code_analyzer',    value = '0')

cfg_file.set_value (section = 'hls', key = 'syn.compile.pipeline_style',    value = 'frp')
cfg_file.set_value (section = 'hls', key = 'syn.dataflow.default_channel',    value = 'fifo')
cfg_file.set_value (section = 'hls', key = 'syn.dataflow.fifo_depth',    value = '16')
cfg_file.set_value (section = 'hls', key = 'syn.directive.interface',    value = 'free_pipe_mult mode=ap_fifo B')
cfg_file.set_value (section = 'hls', key = 'syn.directive.interface',    value = 'free_pipe_mult mode=ap_fifo out')

# Run flow steps
comp = client.get_component(name='using_free_running_pipeline')
comp.run(operation='C_SIMULATION')
comp.run(operation='SYNTHESIS')
comp.run(operation='CO_SIMULATION')
