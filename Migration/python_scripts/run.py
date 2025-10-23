
import vitis
import os

cwd = os.getcwd()+'/'

# Demonstrates use of environment variable inside python script
# Set up SRC_PATH environment variable (if not set the path defaults to ../../../)
# Relative path is wrt to generated hls config file located at <workspace_dir>/<component_dir>/hls_config.cfg
src_path = os.environ.get('SRC_PATH', '../../../')

# Initialize session
client = vitis.create_client()
client.set_workspace(path='./work_pointer_basic')

# Delete the component if it already exists
if os.path.exists('./work_pointer_basic/pointer_basic'):
	client.delete_component(name='pointer_basic')

# Create component. Create new config file in the component folder of the workspace
comp = client.create_hls_component(name='pointer_basic', cfg_file = ['hls_config.cfg'], template = 'empty_hls_component')

# Get handle of config file, then programmatically set desired options
cfg_file = client.get_config_file(path = './work_pointer_basic/pointer_basic/hls_config.cfg')
cfg_file.set_value (                 key = 'part',                  value = 'xcvu9p-flga2104-2-i') 
cfg_file.set_value (section = 'hls', key = 'syn.file',              value = src_path + 'pointer_basic.c')
cfg_file.set_values(section = 'hls', key = 'tb.file',               values = [src_path + 'pointer_basic_test.c',src_path + 'result.golden.dat'])
cfg_file.set_value (section = 'hls', key = 'syn.top',               value = 'pointer_basic')
cfg_file.set_value (section = 'hls', key = 'clock',                 value = '4') # 250MHz
cfg_file.set_value (section = 'hls', key = 'flow_target',           value = 'vivado')
cfg_file.set_value (section = 'hls', key = 'syn.directive.interface',           value = 'pointer_basic mode=m_axi depth=1 d')

# Run flow steps
comp = client.get_component(name='pointer_basic')
comp.run(operation='C_SIMULATION')
comp.run(operation='SYNTHESIS')
comp.run(operation='CO_SIMULATION')
