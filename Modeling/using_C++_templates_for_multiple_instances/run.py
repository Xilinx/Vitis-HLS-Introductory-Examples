import vitis
import os

cwd = os.getcwd()+'/'
# Initialize session
client = vitis.create_client()
client.set_workspace(path='./work_using_C++_templates_for_multiple_instances')

# Delete the component if it already exists
if os.path.exists('./work_using_C++_templates_for_multiple_instances/using_C++_templates_for_multiple_instances'):
	client.delete_component(name='using_C++_templates_for_multiple_instances')

# Create component. Create new config file in the component folder of the workspace
comp = client.create_hls_component(name='using_C++_templates_for_multiple_instances', cfg_file = [cwd+'hls_config.cfg'], template = 'empty_hls_component')

# Run flow steps
comp = client.get_component(name='using_C++_templates_for_multiple_instances')
comp.run(operation='C_SIMULATION')
comp.run(operation='SYNTHESIS')
comp.run(operation='CO_SIMULATION')


