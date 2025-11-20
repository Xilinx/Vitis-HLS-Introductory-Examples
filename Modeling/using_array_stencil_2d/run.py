import vitis
import os

cwd = os.getcwd()+'/'

# Initialize session
client = vitis.create_client()
client.set_workspace(path='./work_stencil_2d')

# Delete the component if it already exists
if os.path.exists('./work_stencil_2d/stencil_2d'):
	client.delete_component(name='stencil_2d')

# Create component. Create new config file in the component folder of the workspace
comp = client.create_hls_component(name='stencil_2d', cfg_file = [cwd+'hls_config.cfg'], template = 'empty_hls_component')

# Run flow steps
comp = client.get_component(name='stencil_2d')
comp.run(operation='C_SIMULATION')
comp.run(operation='SYNTHESIS')
comp.run(operation='CO_SIMULATION')

