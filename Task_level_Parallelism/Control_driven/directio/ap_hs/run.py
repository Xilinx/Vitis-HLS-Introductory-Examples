import vitis
import os

cwd = os.getcwd()+'/'
# Initialize session
client = vitis.create_client()
client.set_workspace(path='./w')

# Delete the component if it already exists
if os.path.exists('./w/directIo'):
	client.delete_component(name='directIo')

# Create component. Create new config file in the component folder of the workspace
comp = client.create_hls_component(name='directIo', cfg_file = [cwd+'config.cfg'], template = 'empty_hls_component')

# Run flow steps
comp = client.get_component(name='directIo')
comp.run(operation='C_SIMULATION')
comp.run(operation='SYNTHESIS')
comp.run(operation='CO_SIMULATION')


