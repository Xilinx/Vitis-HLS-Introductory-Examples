#!/usr/bin/env python3
"""
Plot FFT test signals and their FFT results.
Generates visualizations for all signals in the data directory.
"""

import numpy as np
import matplotlib.pyplot as plt
import os

# FFT size
N = 1024

def read_complex_data(filename):
    """Read complex data from file (real imag format)"""
    data = []
    with open(filename, 'r') as f:
        for line in f:
            parts = line.strip().split()
            if len(parts) >= 2:
                real = float(parts[0])
                imag = float(parts[1])
                data.append(complex(real, imag))
    return np.array(data)

def plot_signal_and_fft(signal_name, input_file, fft_file, output_prefix):
    """Create comprehensive plots for a signal and its FFT"""
    
    # Read data
    input_data = read_complex_data(input_file)
    fft_data = read_complex_data(fft_file)
    
    # Create figure with subplots
    fig = plt.figure(figsize=(16, 10))
    fig.suptitle(f'{signal_name} Signal - Time Domain and FFT Analysis', 
                 fontsize=16, fontweight='bold')
    
    # Time domain - Real part
    ax1 = plt.subplot(3, 2, 1)
    ax1.plot(input_data.real, 'b-', linewidth=0.8)
    ax1.set_xlabel('Sample Index')
    ax1.set_ylabel('Amplitude')
    ax1.set_title('Time Domain - Real Part')
    ax1.grid(True, alpha=0.3)
    
    # Time domain - Imaginary part
    ax2 = plt.subplot(3, 2, 2)
    ax2.plot(input_data.imag, 'r-', linewidth=0.8)
    ax2.set_xlabel('Sample Index')
    ax2.set_ylabel('Amplitude')
    ax2.set_title('Time Domain - Imaginary Part')
    ax2.grid(True, alpha=0.3)
    
    # FFT Magnitude (linear scale)
    ax3 = plt.subplot(3, 2, 3)
    magnitude = np.abs(fft_data)
    ax3.plot(magnitude, 'g-', linewidth=0.8)
    ax3.set_xlabel('Frequency Bin')
    ax3.set_ylabel('Magnitude')
    ax3.set_title('FFT Magnitude (Linear Scale)')
    ax3.grid(True, alpha=0.3)
    ax3.set_xlim([0, N])
    
    # FFT Magnitude (dB scale)
    ax4 = plt.subplot(3, 2, 4)
    magnitude_db = 20 * np.log10(magnitude + 1e-10)  # Add small value to avoid log(0)
    ax4.plot(magnitude_db, 'g-', linewidth=0.8)
    ax4.set_xlabel('Frequency Bin')
    ax4.set_ylabel('Magnitude (dB)')
    ax4.set_title('FFT Magnitude (dB Scale)')
    ax4.grid(True, alpha=0.3)
    ax4.set_xlim([0, N])
    
    # FFT Phase
    ax5 = plt.subplot(3, 2, 5)
    phase = np.angle(fft_data)
    ax5.plot(phase, 'm-', linewidth=0.8)
    ax5.set_xlabel('Frequency Bin')
    ax5.set_ylabel('Phase (radians)')
    ax5.set_title('FFT Phase')
    ax5.grid(True, alpha=0.3)
    ax5.set_xlim([0, N])
    ax5.set_ylim([-np.pi, np.pi])
    
    # Zoomed FFT Magnitude (first 100 bins)
    ax6 = plt.subplot(3, 2, 6)
    ax6.stem(range(100), magnitude[:100], linefmt='b-', markerfmt='bo', basefmt='k-')
    ax6.set_xlabel('Frequency Bin')
    ax6.set_ylabel('Magnitude')
    ax6.set_title('FFT Magnitude - First 100 Bins (Zoomed)')
    ax6.grid(True, alpha=0.3)
    ax6.set_xlim([0, 100])
    
    plt.tight_layout()
    
    # Save figure
    output_file = f'{output_prefix}_{signal_name.lower()}_analysis.png'
    plt.savefig(output_file, dpi=150, bbox_inches='tight')
    print(f"  Saved: {output_file}")
    plt.close()

def create_comparison_plot(output_file='plots/all_signals_comparison.png'):
    """Create comparison plot of all signals"""
    
    signals = [
        ('Ramp', 'data/ramp_input.dat', 'data/ramp_fft_output.dat', 'tab:blue'),
        ('Sine', 'data/sine_input.dat', 'data/sine_fft_output.dat', 'tab:orange'),
        ('Sawtooth', 'data/sawtooth_input.dat', 'data/sawtooth_fft_output.dat', 'tab:green'),
        ('Mixed', 'data/mixed_input.dat', 'data/mixed_fft_output.dat', 'tab:red'),
        ('Impulse', 'data/impulse_input.dat', 'data/impulse_fft_output.dat', 'tab:purple')
    ]
    
    fig, axes = plt.subplots(2, 5, figsize=(20, 8))
    fig.suptitle('All Test Signals Comparison', fontsize=16, fontweight='bold')
    
    for idx, (name, input_file, fft_file, color) in enumerate(signals):
        input_data = read_complex_data(input_file)
        fft_data = read_complex_data(fft_file)
        
        # Time domain plot
        ax_time = axes[0, idx]
        ax_time.plot(input_data.real, color=color, linewidth=0.8)
        ax_time.set_title(f'{name}\nTime Domain')
        ax_time.set_xlabel('Sample')
        ax_time.set_ylabel('Amplitude')
        ax_time.grid(True, alpha=0.3)
        
        # FFT magnitude plot (first 200 bins)
        ax_fft = axes[1, idx]
        magnitude = np.abs(fft_data[:200])
        ax_fft.plot(magnitude, color=color, linewidth=0.8)
        ax_fft.set_title(f'{name}\nFFT Magnitude')
        ax_fft.set_xlabel('Frequency Bin')
        ax_fft.set_ylabel('Magnitude')
        ax_fft.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig(output_file, dpi=150, bbox_inches='tight')
    print(f"  Saved: {output_file}")
    plt.close()

def print_signal_statistics():
    """Print statistics for all signals"""
    
    signals = [
        ('Ramp', 'data/ramp_input.dat', 'data/ramp_fft_output.dat'),
        ('Sine', 'data/sine_input.dat', 'data/sine_fft_output.dat'),
        ('Sawtooth', 'data/sawtooth_input.dat', 'data/sawtooth_fft_output.dat'),
        ('Mixed', 'data/mixed_input.dat', 'data/mixed_fft_output.dat'),
        ('Impulse', 'data/impulse_input.dat', 'data/impulse_fft_output.dat')
    ]
    
    print("\n" + "="*70)
    print("Signal Statistics")
    print("="*70)
    
    for name, input_file, fft_file in signals:
        input_data = read_complex_data(input_file)
        fft_data = read_complex_data(fft_file)
        magnitude = np.abs(fft_data)
        
        print(f"\n{name}:")
        print(f"  Time Domain:")
        print(f"    Min: {np.min(input_data.real):.6f}, Max: {np.max(input_data.real):.6f}")
        print(f"    Mean: {np.mean(input_data.real):.6f}, Std: {np.std(input_data.real):.6f}")
        print(f"  FFT Domain:")
        print(f"    DC component: {magnitude[0]:.2f}")
        print(f"    Peak magnitude: {np.max(magnitude):.2f} at bin {np.argmax(magnitude)}")
        
        # Find top 5 peaks (excluding DC)
        sorted_indices = np.argsort(magnitude[1:])[::-1][:5] + 1
        print(f"    Top 5 bins (excluding DC): {sorted_indices.tolist()}")

# Main script
if __name__ == "__main__":
    print("Generating plots for FFT test signals...")
    print("="*70)
    
    # Create output directory
    os.makedirs('plots', exist_ok=True)
    
    # Define signals
    signals = [
        ('Ramp', 'data/ramp_input.dat', 'data/ramp_fft_output.dat'),
        ('Sine', 'data/sine_input.dat', 'data/sine_fft_output.dat'),
        ('Sawtooth', 'data/sawtooth_input.dat', 'data/sawtooth_fft_output.dat'),
        ('Mixed', 'data/mixed_input.dat', 'data/mixed_fft_output.dat'),
        ('Impulse', 'data/impulse_input.dat', 'data/impulse_fft_output.dat')
    ]
    
    # Generate individual plots
    print("\nGenerating individual signal plots...")
    for name, input_file, fft_file in signals:
        print(f"Processing {name}...")
        plot_signal_and_fft(name, input_file, fft_file, 'plots')
    
    # Generate comparison plot
    print("\nGenerating comparison plot...")
    create_comparison_plot()
    
    # Print statistics
    print_signal_statistics()
    
    print("\n" + "="*70)
    print("All plots generated successfully in 'plots/' directory!")
    print("="*70)
