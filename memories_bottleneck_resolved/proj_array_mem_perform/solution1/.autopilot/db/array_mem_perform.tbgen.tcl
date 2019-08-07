set moduleName array_mem_perform
set isTopModule 1
set isTaskLevelControl 1
set isCombinational 0
set isDatapathOnly 0
set isFreeRunPipelineModule 0
set isPipelined 0
set pipeline_type none
set FunctionProtocol ap_ctrl_hs
set isOneStateSeq 0
set ProfileFlag 0
set StallSigGenFlag 0
set isEnableWaveformDebug 1
set C_modelName {array_mem_perform}
set C_modelType { int 10 }
set C_modelArgList {
	{ mem int 8 regular {bram 128 { 1 1 } 1 1 }  }
}
set C_modelArgMapList {[ 
	{ "Name" : "mem", "interface" : "bram", "bitwidth" : 8, "direction" : "READONLY", "bitSlice":[{"low":0,"up":7,"cElement": [{"cName": "mem","cData": "char","bit_use": { "low": 0,"up": 7},"cArray": [{"low" : 0,"up" : 127,"step" : 1}]}]}]} , 
 	{ "Name" : "ap_return", "interface" : "wire", "bitwidth" : 10,"bitSlice":[{"low":0,"up":9,"cElement": [{"cName": "return","cData": "dout_t","bit_use": { "low": 0,"up": 9},"cArray": [{"low" : 0,"up" : 1,"step" : 0}]}]}]} ]}
# RTL Port declarations: 
set portNum 21
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ mem_Addr_A sc_out sc_lv 32 signal 0 } 
	{ mem_EN_A sc_out sc_logic 1 signal 0 } 
	{ mem_WEN_A sc_out sc_lv 1 signal 0 } 
	{ mem_Din_A sc_out sc_lv 8 signal 0 } 
	{ mem_Dout_A sc_in sc_lv 8 signal 0 } 
	{ mem_Clk_A sc_out sc_logic 1 signal 0 } 
	{ mem_Rst_A sc_out sc_logic 1 signal 0 } 
	{ mem_Addr_B sc_out sc_lv 32 signal 0 } 
	{ mem_EN_B sc_out sc_logic 1 signal 0 } 
	{ mem_WEN_B sc_out sc_lv 1 signal 0 } 
	{ mem_Din_B sc_out sc_lv 8 signal 0 } 
	{ mem_Dout_B sc_in sc_lv 8 signal 0 } 
	{ mem_Clk_B sc_out sc_logic 1 signal 0 } 
	{ mem_Rst_B sc_out sc_logic 1 signal 0 } 
	{ ap_return sc_out sc_lv 10 signal -1 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "mem_Addr_A", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "mem", "role": "Addr_A" }} , 
 	{ "name": "mem_EN_A", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "mem", "role": "EN_A" }} , 
 	{ "name": "mem_WEN_A", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "mem", "role": "WEN_A" }} , 
 	{ "name": "mem_Din_A", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "mem", "role": "Din_A" }} , 
 	{ "name": "mem_Dout_A", "direction": "in", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "mem", "role": "Dout_A" }} , 
 	{ "name": "mem_Clk_A", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "mem", "role": "Clk_A" }} , 
 	{ "name": "mem_Rst_A", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "mem", "role": "Rst_A" }} , 
 	{ "name": "mem_Addr_B", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "mem", "role": "Addr_B" }} , 
 	{ "name": "mem_EN_B", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "mem", "role": "EN_B" }} , 
 	{ "name": "mem_WEN_B", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "mem", "role": "WEN_B" }} , 
 	{ "name": "mem_Din_B", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "mem", "role": "Din_B" }} , 
 	{ "name": "mem_Dout_B", "direction": "in", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "mem", "role": "Dout_B" }} , 
 	{ "name": "mem_Clk_B", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "mem", "role": "Clk_B" }} , 
 	{ "name": "mem_Rst_B", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "mem", "role": "Rst_B" }} , 
 	{ "name": "ap_return", "direction": "out", "datatype": "sc_lv", "bitwidth":10, "type": "signal", "bundle":{"name": "ap_return", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "",
		"CDFG" : "array_mem_perform",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "130", "EstimateLatencyMax" : "130",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "mem", "Type" : "Bram", "Direction" : "I"}]}]}


set ArgLastReadFirstWriteLatency {
	array_mem_perform {
		mem {Type I LastRead 2 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "130", "Max" : "130"}
	, {"Name" : "Interval", "Min" : "131", "Max" : "131"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
]}

set Spec2ImplPortList { 
	mem { bram {  { mem_Addr_A MemPortADDR2 1 32 }  { mem_EN_A MemPortCE2 1 1 }  { mem_WEN_A MemPortWE2 1 1 }  { mem_Din_A MemPortDIN2 1 8 }  { mem_Dout_A MemPortDOUT2 0 8 }  { mem_Clk_A mem_clk 1 1 }  { mem_Rst_A mem_rst 1 1 }  { mem_Addr_B MemPortADDR2 1 32 }  { mem_EN_B MemPortCE2 1 1 }  { mem_WEN_B MemPortWE2 1 1 }  { mem_Din_B MemPortDIN2 1 8 }  { mem_Dout_B MemPortDOUT2 0 8 }  { mem_Clk_B mem_clk 1 1 }  { mem_Rst_B mem_rst 1 1 } } }
}

set busDeadlockParameterList { 
}

# RTL port scheduling information:
set fifoSchedulingInfoList { 
}

# RTL bus port read request latency information:
set busReadReqLatencyList { 
}

# RTL bus port write response latency information:
set busWriteResLatencyList { 
}

# RTL array port load latency information:
set memoryLoadLatencyList { 
}
