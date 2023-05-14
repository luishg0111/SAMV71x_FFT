import numpy as np
import matplotlib.pyplot as plt

# Cargar los datos del archivo de texto
data = np.loadtxt('D:\SAMV71x_FFT\fft_output\Output_Data_500hz.txt')

# Extraer los valores del arreglo "AudioBuffer"
audio_buffer = data['AudioBuffer']

# Graficar los valores de AudioBuffer
plt.plot(audio_buffer)
plt.xlabel('Índice')
plt.ylabel('Valor')
plt.title('AudioBuffer')
plt.show()

# Extraer los valores del arreglo "fft_inputData"
fft_input_data = data['fft_inputData']

# Graficar los valores de fft_inputData
plt.plot(fft_input_data)
plt.xlabel('Índice')
plt.ylabel('Valor')
plt.title('fft_inputData')
plt.show()