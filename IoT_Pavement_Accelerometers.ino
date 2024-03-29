#include <Wire.h>
#include <PL_ADXL355.h>
#include <arduinoFFT.h>

#define SAMPLE_SIZE 256  // Number of samples to take

PL_ADXL355_H adxl355;
ArduinoFFT FFT = ArduinoFFT();

double vReal[SAMPLE_SIZE];
double vImag[SAMPLE_SIZE];

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Wire.begin();
  if (!adxl355.begin()) {
    Serial.println("Failed to find ADXL355Z chip");
    while (1) {
      delay(10);
    }
  }

  adxl355.setRange(ADXL355_RANGE_8G);
}

void loop() {
  // Collect acceleration data along the x-axis
  for (int i = 0; i < SAMPLE_SIZE; i++) {
    vReal[i] = adxl355.readAccelX();
    vImag[i] = 0;
    delay(10);  // Adjust this delay to change the sampling rate
  }

  // Perform FFT on the collected data
  FFT.Windowing(vReal, SAMPLE_SIZE, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLE_SIZE, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLE_SIZE);

  // Find the peak frequency
  double peakFrequency = 0.0;
  double peakMagnitude = 0.0;
  for (int i = 0; i < SAMPLE_SIZE / 2; i++) {
    if (vReal[i] > peakMagnitude) {
      peakMagnitude = vReal[i];
      peakFrequency = i * (double)SAMPLE_FREQ / (double)SAMPLE_SIZE;
    }
  }

  Serial.print("Vibration Frequency: ");
  Serial.print(peakFrequency, 2);
  Serial.println(" Hz");

  delay(1000);  // Wait for 1 second before taking the next sample
}