#include <PL_ADXL355.h>

// Create instances of ADXL355 for the sensors
PL::ADXL355 adxl355_sensor1(2);
PL::ADXL355 adxl355_sensor3(10); 
PL::ADXL355 adxl355_sensor2(3); 
PL::ADXL355 adxl355_sensor4(11); 

#define SAMPLES 128 // Number of samples for calculation
#define SAMPLING_FREQ 50.0 // Sampling frequency in Hz
unsigned int samplingPeriod;

// ADXL355 range: +/- 2 g
auto range = PL::ADXL355_Range::range2g;

void setup() {
  // Initialize ADXL355 sensors
  adxl355_sensor1.begin();
  adxl355_sensor2.begin();
  adxl355_sensor3.begin();
  adxl355_sensor4.begin();

  // Set ADXL355 range for each sensor
  adxl355_sensor1.setRange(range);
  adxl355_sensor2.setRange(range);
  adxl355_sensor3.setRange(range);
  adxl355_sensor4.setRange(range);

  // Enable ADXL355 measurement for each sensor
  adxl355_sensor1.enableMeasurement();
  adxl355_sensor2.enableMeasurement();
  adxl355_sensor3.enableMeasurement();
  adxl355_sensor4.enableMeasurement();

  // Initialize Serial at 115200 bps
  Serial.begin(115200);

  // Wait for Serial to be ready
  while (!Serial);
}

void loop() {
// Checking the frequency for sensor1
double frequency1 = getVibrationFrequency(adxl355_sensor1);
if (frequency1 > 1.5) {
  Serial.print("Car Detected by Sensor 1: ");
  Serial.print(frequency1);    // Print the vibration frequency
  Serial.println(" Hz");
} else {
  Serial.println("No activity detected by Sensor 1");
}

// Checking the frequency for sensor2
double frequency2 = getVibrationFrequency(adxl355_sensor2);
if (frequency2 > 1.5) {
  Serial.print("Car Detected by Sensor 2: ");
  Serial.print(frequency2);    // Print the vibration frequency
  Serial.println(" Hz");
} else {
  Serial.println("No activity detected by Sensor 2");
}

// Checking the frequency for sensor3
double frequency3 = getVibrationFrequency(adxl355_sensor3);
if (frequency3 > 1.5) {
  Serial.print("Car Detected by Sensor 3: ");
  Serial.print(frequency3);    // Print the vibration frequency
  Serial.println(" Hz");
} else {
  Serial.println("No activity detected by Sensor 3");
}

// Checking the frequency for sensor4
double frequency4 = getVibrationFrequency(adxl355_sensor4);
if (frequency4 > 1.5) {
  Serial.print("Car Detected by Sensor 4: ");
  Serial.print(frequency4);    // Print the vibration frequency
  Serial.println(" Hz");
} else {
  Serial.println("No activity detected by Sensor 4");
}

  delay(200); // Delay to prevent flooding the serial output
}

double getVibrationFrequency(PL::ADXL355 &sensor) {
  float x, y, z;
  double meanMagnitude = 0.0;
  double variance = 0.0;

  for (int i = 0; i < SAMPLES; i++) {
    readData(sensor, x, y, z);
    double magnitude = sqrt(x * x + y * y + z * z);
    meanMagnitude += magnitude;
    delay(1000 / SAMPLING_FREQ);
  }
  meanMagnitude /= SAMPLES;

  for (int i = 0; i < SAMPLES; i++) {
    readData(sensor, x, y, z);
    double magnitude = sqrt(x * x + y * y + z * z);
    variance += pow(magnitude - meanMagnitude, 2);
    delay(1000 / SAMPLING_FREQ);
  }
  variance /= SAMPLES;

  return sqrt(variance) * SAMPLING_FREQ / (2 * M_PI);
}

void readData(PL::ADXL355 &sensor, float &x, float &y, float &z) {
  auto accelerations = sensor.getAccelerations();
  x = accelerations.x;
  y = accelerations.y;
  z = accelerations.z;
}
