
import vitis
import os

cwd = os.getcwd()+'/'

# Initialize session
client = vitis.create_client()
client.set_workspace(path='./wspace')

# Delete the component if it already exists
if os.path.exists('./wspace/fft_interface_array'):
	client.delete_component(name='fft_interface_array')

# Create component. Create new config file in the component folder of the workspace
comp = client.create_hls_component(name='fft_interface_array', cfg_file = ['hls_config.cfg'], template = 'empty_hls_component')

# Get handle of config file, then programmatically set desired options
cfg_file = client.get_config_file(path = './wspace/fft_interface_array/hls_config.cfg')
cfg_file.set_value (                 key = 'part',                  value = 'xcvc1902-vsva2197-2MP-e-S') 
cfg_file.set_value (section = 'hls', key = 'syn.file',              value = cwd+'fft_top.cpp')
cfg_file.set_values(section = 'hls', key = 'tb.file',               values = [cwd+'fft_tb.cpp',
                                                                              cwd+'data-ssr2', cwd+'data-ssr4'])
cfg_file.set_value (section = 'hls', key = 'syn.top',               value = 'fft_top')
cfg_file.set_value (section = 'hls', key = 'clock',                 value = '2') # 500MHz
cfg_file.set_value (section = 'hls', key = 'flow_target',           value = 'vivado')
cfg_file.set_value (section = 'hls', key = 'package.output.format', value = 'rtl')

# Run flow steps
comp = client.get_component(name='fft_interface_array')
comp.run(operation='C_SIMULATION')
comp.run(operation='SYNTHESIS')
comp.run(operation='CO_SIMULATION')
comp.run(operation='IMPLEMENTATION')
