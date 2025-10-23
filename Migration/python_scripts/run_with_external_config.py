import vitis
import os

cwd = os.getcwd()+'/'

# Initialize session
client = vitis.create_client()
client.set_workspace(path='./work_pointer_basic')

# Delete the component if it already exists
if os.path.exists('./work_pointer_basic/pointer_basic'):
	client.delete_component(name='pointer_basic')

# Create component. Create new config file in the component folder of the workspace
comp = client.create_hls_component(name='pointer_basic', cfg_file = [cwd+'hls_config.cfg'], template = 'empty_hls_component')

# Run flow steps
comp = client.get_component(name='pointer_basic')
comp.run(operation='C_SIMULATION')
comp.run(operation='SYNTHESIS')
comp.run(operation='CO_SIMULATION')
