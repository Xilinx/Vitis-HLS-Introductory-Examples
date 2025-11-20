# this python script assumes the current directory name (aka basename) is
# - the component name
# - the stem for the main source code +".cpp"
# - the stem for the testbench source +"_test.cpp"
# - the top level HLS function
import vitis
import os  # for getcwd, path and _exit

cwd = os.getcwd()

# here we set default assumed value for this design/component; change as needed
component_name = 'transposed_fir'
workspace_path = './workspace/'
hls_syn_top    = component_name
hls_syn_file   = component_name+'.cpp'
hls_tb_file    = component_name+'_test.cpp'

print("(run.py) component name is " + component_name)
print("(run.py) workspace name is " + workspace_path)

# Initialize session
client = vitis.create_client(workspace=workspace_path)

# Delete the component if it already exists
if os.path.exists(workspace_path+component_name):
	client.delete_component(name=component_name)

# Create component. Create new config file in the component folder of the workspace
comp = client.create_hls_component(name=component_name, template = 'empty_hls_component')
# Get handle of config file, then programmatically set desired options
cfg_file = client.get_config_file(path = workspace_path+component_name+'/'+'hls_config.cfg')

cfg_file.set_value (                 key = 'part',                  value = 'xcvp1702-vsva3340-3HP-e-S')
cfg_file.set_value (section = 'hls', key = 'syn.file',              value =  cwd+'/'+hls_syn_file)
cfg_file.set_values(section = 'hls', key = 'tb.file',               values= [cwd+'/'+hls_tb_file] )
cfg_file.set_value (section = 'hls', key = 'syn.top',               value = hls_syn_top)
cfg_file.set_value (section = 'hls', key = 'clock',                 value = '800MHz')
cfg_file.set_value (section = 'hls', key = 'flow_target',           value = 'vivado')
cfg_file.set_value (section = 'hls', key = 'package.output.syn',    value = '0')
cfg_file.set_value (section = 'hls', key = 'csim.code_analyzer',    value = '0')

# Run flow steps
comp = client.get_component(name=component_name)
comp.run(operation='C_SIMULATION')
comp.run(operation='SYNTHESIS')
comp.run(operation='CO_SIMULATION')
