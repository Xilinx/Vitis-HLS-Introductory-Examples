#!/usr/bin/env python3
"""
Generate test signals for 1,024-point FFT and save results.
Creates multiple test patterns: ramp, sine wave, and sawtooth (seesaw).
"""

import numpy as np
import os

# FFT size
N = 1024

# Create output directory if it doesn't exist
os.makedirs('data', exist_ok=True)

def save_complex_data(filename, data):
    """Save complex data in format: real imag (one per line)"""
    with open(filename, 'w') as f:
        for val in data:
            f.write(f"{val.real:.6f} {val.imag:.6f}\n")

def save_magnitude_phase(filename, data):
    """Save FFT results as magnitude and phase"""
    with open(filename, 'w') as f:
        for val in data:
            mag = np.abs(val)
            phase = np.angle(val)
            f.write(f"{mag:.6f} {phase:.6f}\n")

# 1. Generate RAMP signal (linear increase from 0 to N-1)
print("Generating ramp signal...")
ramp = np.arange(N, dtype=np.float64)
ramp_complex = ramp + 0j  # Convert to complex
save_complex_data('data/ramp_input.dat', ramp_complex)

# Compute FFT of ramp
ramp_fft = np.fft.fft(ramp_complex)
save_complex_data('data/ramp_fft_output.dat', ramp_fft)
save_magnitude_phase('data/ramp_fft_mag_phase.dat', ramp_fft)
print(f"  Ramp: DC component = {np.abs(ramp_fft[0]):.2f}")

# 2. Generate SINE WAVE signal (single frequency)
print("Generating sine wave signal...")
freq_bins = 10  # Frequency bin (10 cycles over N points)
t = np.arange(N)
sine = np.sin(2 * np.pi * freq_bins * t / N)
sine_complex = sine + 0j
save_complex_data('data/sine_input.dat', sine_complex)

# Compute FFT of sine wave
sine_fft = np.fft.fft(sine_complex)
save_complex_data('data/sine_fft_output.dat', sine_fft)
save_magnitude_phase('data/sine_fft_mag_phase.dat', sine_fft)
peak_bin = np.argmax(np.abs(sine_fft[:N//2]))
print(f"  Sine: Peak at bin {peak_bin} with magnitude {np.abs(sine_fft[peak_bin]):.2f}")

# 3. Generate SAWTOOTH/SEESAW signal (periodic ramp)
print("Generating sawtooth (seesaw) signal...")
period = 64  # Period of sawtooth
sawtooth = np.mod(np.arange(N), period) / period  # Normalized sawtooth 0 to 1
sawtooth_complex = sawtooth + 0j
save_complex_data('data/sawtooth_input.dat', sawtooth_complex)

# Compute FFT of sawtooth
sawtooth_fft = np.fft.fft(sawtooth_complex)
save_complex_data('data/sawtooth_fft_output.dat', sawtooth_fft)
save_magnitude_phase('data/sawtooth_fft_mag_phase.dat', sawtooth_fft)
print(f"  Sawtooth: DC component = {np.abs(sawtooth_fft[0]):.2f}")

# 4. Generate MIXED signal (combination of sine waves)
print("Generating mixed signal (multiple frequencies)...")
freq1, freq2, freq3 = 10, 25, 50
mixed = (np.sin(2 * np.pi * freq1 * t / N) + 
         0.5 * np.sin(2 * np.pi * freq2 * t / N) +
         0.3 * np.sin(2 * np.pi * freq3 * t / N))
mixed_complex = mixed + 0j
save_complex_data('data/mixed_input.dat', mixed_complex)

# Compute FFT of mixed signal
mixed_fft = np.fft.fft(mixed_complex)
save_complex_data('data/mixed_fft_output.dat', mixed_fft)
save_magnitude_phase('data/mixed_fft_mag_phase.dat', mixed_fft)
print(f"  Mixed: Peaks at bins {freq1}, {freq2}, {freq3}")

# 5. Generate IMPULSE signal (unit impulse at t=0)
print("Generating impulse signal...")
impulse = np.zeros(N, dtype=np.float64)
impulse[0] = 1.0
impulse_complex = impulse + 0j
save_complex_data('data/impulse_input.dat', impulse_complex)

# Compute FFT of impulse
impulse_fft = np.fft.fft(impulse_complex)
save_complex_data('data/impulse_fft_output.dat', impulse_fft)
save_magnitude_phase('data/impulse_fft_mag_phase.dat', impulse_fft)
print(f"  Impulse: All bins should have magnitude ~1.0")

# Print summary
print("\nGeneration complete!")
print(f"All files saved in 'data/' directory")
print(f"\nGenerated signals:")
print(f"  - Ramp: Linear increase from 0 to {N-1}")
print(f"  - Sine: Single frequency at bin {freq_bins}")
print(f"  - Sawtooth: Periodic ramp with period {period}")
print(f"  - Mixed: Three sine waves at bins {freq1}, {freq2}, {freq3}")
print(f"  - Impulse: Unit impulse at t=0")
print(f"\nFor each signal, generated:")
print(f"  - *_input.dat: Time domain input (real imag)")
print(f"  - *_fft_output.dat: FFT result (real imag)")
print(f"  - *_fft_mag_phase.dat: FFT result (magnitude phase)")
