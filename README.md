# Audio Signal Analyzer

An audio signal processing and visualization application developed in **ANSI C using NI LabWindows/CVI**, with **Python** scripts for preparing WAV data. The application explores audio signals in both the time and frequency domains through statistical measurements, digital filters, and power-spectrum analysis.

## Technologies Used

- **C (LabWindows/CVI):** graphical interface, data processing, signal visualization, and filtering
- **NI Analysis Library:** statistical functions, windowing, and spectral analysis
- **Python (NumPy, SciPy):** WAV file loading, sample extraction, and amplitude-envelope calculation

## Features

### Time-Domain Analysis

- Display the audio waveform in one-second intervals.
- Calculate minimum, maximum, mean, median, and variance.
- Calculate skewness and kurtosis, and count zero crossings.
- Display a signal histogram and an amplitude envelope derived using the Hilbert transform.
- Apply selectable processing operations:
  - **Moving-average filter** with a configurable window size.
  - **First-order low-pass (alpha) filter** with a configurable coefficient.
  - **Discrete first difference** between adjacent samples.
- Compare the original and processed signals in the graphical interface.

### Frequency-Domain Analysis

- Analyze successive segments of a loaded signal using the NI Analysis Library's FFT-based power-spectrum functions.
- Apply selectable windowing functions before spectral analysis.
- Estimate the **dominant frequency** and its corresponding peak power.
- Display the power spectrum for the original or filtered signal.
- Export waveform, histogram, and spectral plots as JPEG images.

## How It Works

1. **Prepare the audio data:** `main.py` loads the sample WAV file, extracts samples, computes the amplitude envelope, and writes text files for the C application.
2. **Load and visualize:** the LabWindows/CVI interface reads the generated files and displays the waveform and time-domain measurements.
3. **Process the signal:** the user can apply a moving-average filter, a first-order filter, or a discrete first-difference operation.
4. **Explore frequencies:** the interface processes successive signal segments and displays the resulting power spectrum and dominant frequency.

## Running the Project

### Requirements

- Windows with **NI LabWindows/CVI** and its Analysis and User Interface libraries
- **Python 3** accessible from the command line, with NumPy and SciPy installed
- An input WAV file (the current scripts use `52.wav`)

Install the Python dependencies:

```bash
python -m pip install numpy scipy
```

### Steps

1. Open `proiect.prj` in LabWindows/CVI.
2. Keep `Interface.uir`, the C sources, and `main.py` in the project working directory.
3. Build and run the application in LabWindows/CVI.
4. Use **Load Wave** to invoke `main.py`, load the generated signal data, and display the waveform.
5. Use the filter controls to compare processed and original signals; open the frequency-analysis view to inspect the spectrum.

**Current implementation details:** `main.py` uses the fixed filename `52.wav`. To analyze another file, change the filename in the script. The script also appends to `waveInfo.txt`; clear or remove that generated file before a new run so that outdated metadata is not read. The data preparation step and GUI currently depend on files in the working directory.

## Project Structure

| File | Purpose |
| --- | --- |
| [`Interface.c`](Interface.c) | Main LabWindows/CVI application, processing logic, and GUI callbacks |
| [`Interface.uir`](Interface.uir) | Graphical user interface resource |
| [`Interface.h`](Interface.h) | UI-generated control identifiers |
| [`main.py`](main.py) | WAV sample extraction and Hilbert-envelope preprocessing |
| [`check.py`](check.py) | WAV metadata inspection utility |
| [`proiect.prj`](proiect.prj) | LabWindows/CVI project configuration |
| [`52.wav`](52.wav) | Example input audio file |
| [`Documentatie.pdf`](Documentatie.pdf) | Detailed project documentation |

The repository also includes generated text data and a compiled executable from the development environment.

## Implementation Highlights

- The moving-average filter computes the average of the latest `N` samples for each valid window.
- The first-order low-pass filter uses the recursive relation `y[n] = (1 - alpha) * y[n-1] + alpha * x[n]`, with `0 < alpha < 1`.
- Spectral estimation uses the NI functions `ScaledWindowEx`, `AutoPowerSpectrum`, and `PowerFrequencyEstimate` rather than a custom FFT implementation.
- The GUI uses event callbacks for loading, processing, navigation, and chart export.
