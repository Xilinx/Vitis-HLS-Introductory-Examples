
import vitis
import os

cwd = os.getcwd()+'/'

# Initialize session
client = vitis.create_client()
client.set_workspace(path='./w')

# Delete the component if it already exists
if os.path.exists('./w/rtl_as_blackbox'):
	client.delete_component(name='rtl_as_blackbox')

# Create component. Create new config file in the component folder of the workspace
comp = client.create_hls_component(name='rtl_as_blackbox', cfg_file = ['hls_config.cfg'], template = 'empty_hls_component')

# Get handle of config file, then programmatically set desired options
cfg_file = client.get_config_file(path = './w/rtl_as_blackbox/hls_config.cfg')
cfg_file.set_value (                 key = 'part',                  value = 'xcvu9p-flga2104-2-i') 
cfg_file.set_value (section = 'hls', key = 'syn.file',              value = cwd+'example.cpp')
cfg_file.set_value (section = 'hls', key = 'syn.blackbox.file',     value = cwd+'rtl_model.json')
cfg_file.set_values(section = 'hls', key = 'tb.file',               values = [cwd+'example_test.cpp', cwd+'result.golden.dat'])
cfg_file.set_value (section = 'hls', key = 'syn.top',               value = 'example')
cfg_file.set_value (section = 'hls', key = 'clock',                 value = '4') # 250MHz
cfg_file.set_value (section = 'hls', key = 'flow_target',           value = 'vivado')
cfg_file.set_value (section = 'hls', key = 'package.output.format', value = 'rtl')
cfg_file.set_value (section = 'hls', key = 'csim.code_analyzer',    value = '0')

# Run flow steps
comp = client.get_component(name='rtl_as_blackbox')
comp.run(operation='C_SIMULATION')
comp.run(operation='SYNTHESIS')
comp.run(operation='CO_SIMULATION')
