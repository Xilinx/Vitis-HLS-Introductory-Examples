#!/bin/bash

# Comprehensive test script for all 8 hardblock DSP examples with place & route
# Tests both FFT and LDPC examples using Tcl and Python methods
# Includes IMPLEMENTATION and EXPORT phases (place & route)

set -e

# Setup
REPO_DIR="$(pwd)"
TEST_DIR="${REPO_DIR}/comprehensive_tests_placement"
mkdir -p "${TEST_DIR}"

# Load Vitis 2026.2
if type use &>/dev/null; then
    use 2026.2
else
    echo "Warning: 'use' function not found. Assuming Vitis 2026.2 is already configured."
fi

# Define test cases: (name, directory, method)
declare -a TESTS=(
    "FFT_BASE_TCL:DSP/fft/LogiCORE-versal-hardblock-fft/versal_rf_fft_base:tcl"
    "FFT_BASE_PYTHON:DSP/fft/LogiCORE-versal-hardblock-fft/versal_rf_fft_base:python"
    "FFT_FWD_INV_TCL:DSP/fft/LogiCORE-versal-hardblock-fft/versal_rf_fft_forward_inverse:tcl"
    "FFT_FWD_INV_PYTHON:DSP/fft/LogiCORE-versal-hardblock-fft/versal_rf_fft_forward_inverse:python"
    "LDPC_5G_TCL:DSP/ldpc/LogiCORE-versal-ldpc-hardblock/ldpc_5g:tcl"
    "LDPC_5G_PYTHON:DSP/ldpc/LogiCORE-versal-ldpc-hardblock/ldpc_5g:python"
    "LDPC_WORDS_TCL:DSP/ldpc/LogiCORE-versal-ldpc-hardblock/ldpc_words:tcl"
    "LDPC_WORDS_PYTHON:DSP/ldpc/LogiCORE-versal-ldpc-hardblock/ldpc_words:python"
)

# Track results
declare -A RESULTS
declare -a PIDS
declare -a NAMES

# Run test function
run_test() {
    local name="$1"
    local dir="$2"
    local method="$3"
    local log_dir="${TEST_DIR}/${name}"
    local log_file="${log_dir}/${name}.log"

    mkdir -p "${log_dir}"

    cd "${REPO_DIR}/${dir}"

    echo "Starting ${name} (${method})..."

    if [ "${method}" = "tcl" ]; then
        # Run Tcl-based flow
        vitis-run --mode hls --tcl run_hls.tcl > "${log_file}" 2>&1
    else
        # Run Python-based flow (use 'vitis -s' to provide proper Python environment)
        vitis -s run.py > "${log_file}" 2>&1
    fi

    # Check for success
    if grep -q "C/RTL co-simulation finished: PASS" "${log_file}"; then
        RESULTS["${name}"]="PASS"
        echo "✓ ${name} - PASS"
    elif grep -q "export_design finished successfully" "${log_file}" || \
         grep -q "EXPORT completed successfully" "${log_file}"; then
        # Check for IMPLEMENTATION/EXPORT completion
        RESULTS["${name}"]="PASS"
        echo "✓ ${name} - PASS (placement & route completed)"
    elif grep -q "ERROR" "${log_file}"; then
        RESULTS["${name}"]="FAIL"
        echo "✗ ${name} - FAIL"
    else
        RESULTS["${name}"]="UNKNOWN"
        echo "? ${name} - UNKNOWN"
    fi
}

echo "=========================================="
echo "Comprehensive Hardblock DSP Test Suite"
echo "With Place & Route (IMPLEMENTATION/EXPORT)"
echo "=========================================="
echo ""
echo "Running 8 tests in parallel..."
echo "- FFT Base (Tcl + Python)"
echo "- FFT Forward/Inverse (Tcl + Python)"
echo "- LDPC 5G (Tcl + Python)"
echo "- LDPC Words (Tcl + Python)"
echo ""

# Start all tests in parallel
start_time=$(date +%s)

for test_spec in "${TESTS[@]}"; do
    IFS=':' read -r name dir method <<< "${test_spec}"
    run_test "${name}" "${dir}" "${method}" &
    PIDS+=($!)
    NAMES+=("${name}")
done

# Wait for all tests to complete
for i in "${!PIDS[@]}"; do
    wait "${PIDS[$i]}" || true
done

end_time=$(date +%s)
elapsed=$((end_time - start_time))

# Print results
echo ""
echo "=========================================="
echo "Test Results Summary"
echo "=========================================="
echo ""

pass_count=0
fail_count=0
unknown_count=0

for name in "${NAMES[@]}"; do
    status="${RESULTS[$name]}"
    case "${status}" in
        PASS)
            echo "✓ ${name}: PASS"
            ((pass_count++))
            ;;
        FAIL)
            echo "✗ ${name}: FAIL"
            ((fail_count++))
            ;;
        UNKNOWN)
            echo "? ${name}: UNKNOWN"
            ((unknown_count++))
            ;;
    esac
done

echo ""
echo "Results: ${pass_count}/8 PASS"
echo "Time: ${elapsed}s (~$((elapsed/60)) minutes)"
echo ""

if [ ${fail_count} -eq 0 ] && [ ${unknown_count} -eq 0 ]; then
    echo "Status: ✓ ALL TESTS PASSED - PLACEMENT & ROUTE SUCCESSFUL"
    exit 0
elif [ ${pass_count} -ge 6 ]; then
    echo "Status: ⚠ MOSTLY PASSED - Review any FAIL/UNKNOWN results"
    exit 1
else
    echo "Status: ✗ MULTIPLE FAILURES - Review logs for details"
    exit 2
fi
