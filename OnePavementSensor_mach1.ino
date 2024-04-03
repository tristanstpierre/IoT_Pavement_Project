#include <PL_ADXL355.h>

// Create instances of ADXL355 for each sensor
// Assign unique CS pins for each sensor, e.g., 2 and 3
PL::ADXL355 adxl355_sensor1(2);
float x,y,z;
#define SAMPLES 128 // Number of samples for the FFT
#define SAMPLING_FREQ 50.0 // Sampling frequency in Hz
unsigned int samplingPeriod;
unsigned long microseconds;

// ADXL355 range: +/- 2 g
auto range = PL::ADXL355_Range::range2g;

void setup() {
  // Initialize both ADXL355 sensors
  adxl355_sensor1.begin();
  // Set ADXL355 range for both sensors
  adxl355_sensor1.setRange(range);
  // Enable ADXL355 measurement for both sensors
  adxl355_sensor1.enableMeasurement();
  // Initialize Serial at 115200 kbps
  Serial.begin(115200);
  // Wait for Serial to be ready
  while(!Serial);
}

void loop() {
  Serial.print("Vibration Frequency: "); 
  Serial.print(getVibrationFrequency());    // print the vibration frequency
  Serial.println(" Hz");
  delay(200);
}

double getVibrationFrequency() {
  readData();
  
  // Calculate the magnitude of the acceleration vector
  double magnitude = sqrt(x * x + y * y + z * z);

  // Compute the mean of the magnitude
  double meanMagnitude = 0.0;
  for (int i = 0; i < SAMPLES; i++) {
    readData(); // Read the accelerometer data
    magnitude = sqrt(x * x + y * y + z * z);
    meanMagnitude += magnitude;
    delay(1000 / SAMPLING_FREQ); // Wait for the next sample
  }
  meanMagnitude /= SAMPLES;

  // Compute the variance of the magnitude
  double variance = 0.0;
  for (int i = 0; i < SAMPLES; i++) {
    readData(); // Read the accelerometer data
    magnitude = sqrt(x * x + y * y + z * z);
    variance += pow(magnitude - meanMagnitude, 2);
    delay(1000 / SAMPLING_FREQ); // Wait for the next sample
  }
  variance /= SAMPLES;

  // Compute the vibration frequency from the variance
  double vibrationFrequency = sqrt(variance) * SAMPLING_FREQ / (2 * M_PI);

  return vibrationFrequency;
}

// function readData
void readData() {
  // Read the accelerations for the first sensor
  auto accelerations1 = adxl355_sensor1.getAccelerations();
  x = accelerations1.x;
  y = accelerations1.y;
  z = accelerations1.z;
}
