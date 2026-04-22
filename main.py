import numpy as np

from scipy.io import wavfile
from scipy.signal import hilbert

samplerate, data = wavfile.read("52.wav")

if len(data.shape)>1:
    data=data[:, 0]
data=data.astype(float)

analytic_signal = hilbert(data)
amplitude_envelope = np.abs(analytic_signal)

wavFileInfo = open("waveInfo.txt", "a")
wavFileInfo.write(str(samplerate)+'\n')
wavFileInfo.write(str(data.size)+'\n')
wavFileInfo.close()

print(samplerate)
print(data.size)
print (data)

np.savetxt("waveData.txt", data, fmt="%2.0f")

np.savetxt("envelope.txt", amplitude_envelope, fmt="%.4f")
