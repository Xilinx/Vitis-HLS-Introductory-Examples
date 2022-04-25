TARGET := hw

build: diamond.$(TARGET).xclbin host

diamond.$(TARGET).xo: ../using_fifos/diamond.cpp
	v++ -c -g -t $(TARGET) -R 1 -k diamond \
		--save-temps \
		--temp_dir ./temp_dir \
		--report_dir ./report_dir \
		--log_dir ./log_dir \
		--config ./options.cfg \
		-I. \
		../using_fifos/diamond.cpp \
		-o ./diamond.$(TARGET).xo

diamond.$(TARGET).xclbin: diamond.$(TARGET).xo
	v++ -l -g -t $(TARGET) -R 1 \
		--temp_dir ./temp_dir \
		--report_dir ./report_dir \
		--log_dir ./log_dir \
		--config ./options.cfg \
		-I. \
		diamond.$(TARGET).xo \
		-o diamond.$(TARGET).xclbin

host: ./diamond_host.cpp
	mkdir -p build/
	g++ -D__USE_XOPEN2K8 -D__USE_XOPEN2K8 \
		-I$(XILINX_XRT)/include/ \
		-I. \
		-O3 -Wall -fmessage-length=0 -std=c++1y\
		./diamond_host.cpp \
		-L$(XILINX_XRT)/lib/ \
		-lxrt_coreutil -lpthread \
		-o ./host

xclbin: diamond.xclbin

xo: diamond.xo

run: build
	@echo "Running $(TAGET) mode"; 
ifeq ($(TARGET), hw)
	@echo "************  Use Command Line to run application!  ************"
	./host ./diamond.hw.xclbin ;
else
	@echo "Running $(TAGET) mode"; 
	emconfigutil --nd 1  --platform  xilinx_u250_gen3x16_xdma_3_1_202020_1 --od ./
	XCL_EMULATION_MODE=$(TARGET) ./host ./diamond.$(TARGET).xclbin ;
endif

clean:
	rm -rf temp_dir log_dir report_dir *log *.csv diamond.hw.ltx  diamond.hw.xclbin.info  emconfig.json

cleanall: clean
	rm -rf diamond.*.xo diamond.*.xclbin *summary

