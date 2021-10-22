VPP := $(XILINX_VITIS)/bin/v++
EMCONFIGUTIL := $(XILINX_VITIS)/bin/emconfigutil
TARGET:= hw
DEVICE:= xilinx_u250_gen3x16_xdma_3_1_202020_1

# kernel targets
KRNL_XO := kernel.$(TARGET).xo
XCLBIN := kernel.$(TARGET).xclbin

# host target
HOST_EXE := host.exe

# config files target
EMCONFIG_FILE := emconfig.json

VPP_OPTS := -s -t $(TARGET) --platform $(DEVICE)
VPP_OPTS += --temp_dir ./all_builds/$(TARGET)
VPP_OPTS += --report_dir ./all_logs/$(TARGET)
VPP_OPTS += --log_dir ./all_logs/$(TARGET)

CFLAGS := -g -std=c++1y -I$(XILINX_XRT)/include 
LFLAGS := -L$(XILINX_XRT)/lib -lOpenCL -lxrt_coreutil -lrt -luuid

# run time args
CMD_ARGS:= kernel.${TARGET}.xclbin

# primary build targets
.PHONY: xclbin host all

xclbin:  $(XCLBIN)
host: $(HOST_EXE) 

all: xclbin host 

clean:
	-$(RM) $(EMCONFIG_FILE) $(HOST_EXE) $(XCLBIN) 

cleanall: clean	
	rm -rf all_* *.log *.xclbin *.compile_summary *.link_summary *.info *.xo

# kernel rules
$(KRNL_XO): ./vadd_kernel/krnl_vadd.cpp
	$(RM) $@
	$(VPP) $(VPP_OPTS) -c -k krnl_vadd -I'$(<D)' -o $@ $+

$(XCLBIN): $(KRNL_XO) 
	$(VPP) $(VPP_OPTS) -l -o $@ $(KRNL_XO) $+ 

# host rules
$(HOST_EXE): ./host/host.cpp 
	g++ $(CFLAGS) -o $@ $+ $(LFLAGS)
	@echo 'Compiled Host Executable: $(HOST_EXE)'

$(EMCONFIG_FILE):
	$(EMCONFIGUTIL) --nd 1 --od . --platform $(DEVICE)

run: $(XCLBIN) $(HOST_EXE) $(EMCONFIG_FILE)
	XCL_EMULATION_MODE=${TARGET} ./$(HOST_EXE) $(CMD_ARGS)
