
import vitis
import os

cwd = os.getcwd()+'/'

# Initialize session
client = vitis.create_client()
client.set_workspace(path='./w')

# Delete the component if it already exists
if os.path.exists('./w/fft_interface_stream'):
	client.delete_component(name='fft_interface_stream')

# Create component. Create new config file in the component folder of the workspace
comp = client.create_hls_component(name='fft_interface_stream', cfg_file = ['hls_config.cfg'], template = 'empty_hls_component')

# Get handle of config file, then programmatically set desired options
cfg_file = client.get_config_file(path = './w/fft_interface_stream/hls_config.cfg')
cfg_file.set_value (                 key = 'part',                  value = 'xcvu9p-flga2104-2-i') 
cfg_file.set_value (section = 'hls', key = 'syn.file',              value = cwd+'fft_top.cpp')
cfg_file.set_values(section = 'hls', key = 'tb.file',               values = [cwd+'fft_tb.cpp', 
									cwd+'data/stimulus_00.dat', cwd+'data/stimulus_01.dat',
									cwd+'data/stimulus_02.dat', cwd+'data/stimulus_03.dat',
									cwd+'data/stimulus_04.dat', cwd+'data/stimulus_05.dat',
									cwd+'data/stimulus_06.dat', cwd+'data/stimulus_07.dat',
									cwd+'data/stimulus_08.dat', cwd+'data/stimulus_09.dat',
									cwd+'data/stimulus_10.dat', cwd+'data/stimulus_11.dat',
									cwd+'data/stimulus_12.dat', cwd+'data/stimulus_13.dat',
									cwd+'data/stimulus_14.dat', cwd+'data/stimulus_15.dat',
									cwd+'data/stimulus_16.dat', cwd+'data/stimulus_17.dat',
									cwd+'data/stimulus_18.dat', cwd+'data/stimulus_19.dat',
									cwd+'data/stimulus_00.res', cwd+'data/stimulus_01.res',
									cwd+'data/stimulus_02.res', cwd+'data/stimulus_03.res',
									cwd+'data/stimulus_04.res', cwd+'data/stimulus_05.res',
									cwd+'data/stimulus_06.res', cwd+'data/stimulus_07.res',
									cwd+'data/stimulus_08.res', cwd+'data/stimulus_09.res',
									cwd+'data/stimulus_10.res', cwd+'data/stimulus_11.res',
									cwd+'data/stimulus_12.res', cwd+'data/stimulus_13.res',
									cwd+'data/stimulus_14.res', cwd+'data/stimulus_15.res',
									cwd+'data/stimulus_16.res', cwd+'data/stimulus_17.res',
									cwd+'data/stimulus_18.res', cwd+'data/stimulus_19.res'])
cfg_file.set_value (section = 'hls', key = 'syn.top',               value = 'fft_top')
cfg_file.set_value (section = 'hls', key = 'clock',                 value = '4') # 250MHz
cfg_file.set_value (section = 'hls', key = 'flow_target',           value = 'vivado')
cfg_file.set_value (section = 'hls', key = 'package.output.format', value = 'rtl')

# Run flow steps
comp = client.get_component(name='fft_interface_stream')
comp.run(operation='C_SIMULATION')
comp.run(operation='SYNTHESIS')
comp.run(operation='CO_SIMULATION')
