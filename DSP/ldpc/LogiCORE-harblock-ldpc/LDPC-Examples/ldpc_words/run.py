
import vitis
import os

cwd = os.getcwd()+'/'

# Initialize session
client = vitis.create_client()
client.set_workspace(path='./work_ldpc_words')

# Delete the component if it already exists
if os.path.exists('./work_ldpc_words/comp_ldpc_words'):
	client.delete_component(name='comp_ldpc_words')

# Create component. Create new config file in the component folder of the workspace
comp = client.create_hls_component(name='comp_ldpc_words', cfg_file = ['hls_config.cfg'], template = 'empty_hls_component')

# Get handle of config file, then programmatically set desired options
cfg_file = client.get_config_file(path = './work_ldpc_words/comp_ldpc_words/hls_config.cfg')
cfg_file.set_value (                 key = 'part',     value = 'xcvr1602-vsva2488-2HP-i-S')
cfg_file.set_value (section = 'hls', key = 'syn.file', value = cwd+'ldpc_top.cpp')
cfg_file.set_value (section = 'hls', key = 'tb.file',  value = cwd+'ldpc_tb.cpp')
cfg_file.set_value (section = 'hls', key = 'syn.top',               value = 'ldpc_top')
cfg_file.set_value (section = 'hls', key = 'clock',                 value = '2') # 500MHz
cfg_file.set_value (section = 'hls', key = 'enableBetaDevice',      value = '1')
cfg_file.set_value (section = 'hls', key = 'package.output.format', value = 'rtl')

# Run flow steps
comp = client.get_component(name='comp_ldpc_words')
comp.run(operation='C_SIMULATION')
comp.run(operation='SYNTHESIS')
comp.run(operation='CO_SIMULATION')
