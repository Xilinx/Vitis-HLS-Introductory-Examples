# Variables
HLS_CONFIG = hls_config.cfg
WORK_DIR = w

# Default target
.PHONY: all
all: csim csynth cosim

# Help target
.PHONY: help
help:
	@echo "Usage:"
	@echo "  make csim       - Run C Simulation using vitis-run"
	@echo "  make csynth     - Run C Synthesis using v++"
	@echo "  make cosim      - Run Co-Simulation using vitis-run"
	@echo "  make all        - Run C Simulation, C Synthesis and Co-Simulation"
	@echo "  make clean      - Clean the work directory"

# C Simulation target
.PHONY: csim
csim:
	@echo "Running C Simulation using vitis-run"
	vitis-run --mode hls --csim --config $(HLS_CONFIG) --work_dir $(WORK_DIR)

# C Synthesis target
.PHONY: csynth
csynth:
	@echo "Running C Synthesis using v++"
	v++ --compile --mode hls --config $(HLS_CONFIG) --work_dir $(WORK_DIR)

# Co-Simulation target
.PHONY: cosim
cosim:
	@echo "Running Co-Simulation using vitis-run"
	vitis-run --mode hls --cosim --config $(HLS_CONFIG) --work_dir $(WORK_DIR)

# Clean target
.PHONY: clean
clean:
	@echo "Cleaning work directory"
	rm -rf $(WORK_DIR)
