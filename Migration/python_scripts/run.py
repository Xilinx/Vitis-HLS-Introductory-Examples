
import vitis
import os

cwd = os.getcwd()+'/'

# Initialize session
client = vitis.create_client()
client.set_workspace(path='./w')

# Delete the component if it already exists
if os.path.exists('./w/pointer_basic'):
	client.delete_component(name='pointer_basic')

# Create component. Create new config file in the component folder of the workspace
comp = client.create_hls_component(name='pointer_basic', cfg_file = ['hls_config.cfg'], template = 'empty_hls_component')

# Get handle of config file, then programmatically set desired options
cfg_file = client.get_config_file(path = './w/pointer_basic/hls_config.cfg')
cfg_file.set_value (                 key = 'part',                  value = 'xcvu9p-flga2104-2-i') 
cfg_file.set_value (section = 'hls', key = 'syn.file',              value = '../../../pointer_basic.c')
cfg_file.set_values(section = 'hls', key = 'tb.file',               values = ['../../../pointer_basic_test.c','../../../result.golden.dat'])
cfg_file.set_value (section = 'hls', key = 'syn.top',               value = 'pointer_basic')
cfg_file.set_value (section = 'hls', key = 'clock',                 value = '4') # 250MHz
cfg_file.set_value (section = 'hls', key = 'flow_target',           value = 'vivado')
cfg_file.set_value (section = 'hls', key = 'syn.directive.interface',           value = 'pointer_basic mode=m_axi depth=1 d')

# Run flow steps
comp = client.get_component(name='pointer_basic')
comp.run(operation='C_SIMULATION')
comp.run(operation='SYNTHESIS')
comp.run(operation='CO_SIMULATION')
