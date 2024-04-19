#include <PL_ADXL355.h>
#include <HardwareSerial.h>


// --------------------- ADXL355 ---------------------


// Create instances of ADXL355 for the sensors
PL::ADXL355 adxl355_sensor1(2);
PL::ADXL355 adxl355_sensor3(0); 
PL::ADXL355 adxl355_sensor2(9); 
PL::ADXL355 adxl355_sensor4(10); 

#define SAMPLES 128 // Number of samples for calculation
#define SAMPLING_FREQ 50.0 // Sampling frequency in Hz
unsigned int samplingPeriod;

// ADXL355 range: +/- 2 g
auto range = PL::ADXL355_Range::range2g;




// --------------------- LoraWAN ---------------------

// Define LoRaWAN module UART pins
// #define LORAWAN_RX_PIN 16
// #define LORAWAN_TX_PIN 17
#define LORAWAN_RX_PIN 3
#define LORAWAN_TX_PIN 1

// Define LoRaWAN module baud rate
#define LORAWAN_BAUD_RATE 115200

HardwareSerial lorawanSerial(1);

void setup() {

  // --------------------- ADXL355 ---------------------


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


  // --------------------- LoraWAN ---------------------


  // Initialize LoRaWAN module Serial
  lorawanSerial.begin(LORAWAN_BAUD_RATE, SERIAL_8N1, LORAWAN_RX_PIN, LORAWAN_TX_PIN);

  lorawanSerial.println("AT");
  // Serial.print("Received: ");
  
  lorawanSerial.println("AT+RESET");
  // Serial.print("Received: ");







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

    frequency1 = 0;
  }

  // Checking the frequency for sensor2
  double frequency2 = getVibrationFrequency(adxl355_sensor2);
  if (frequency2 > 1.5) {
    Serial.print("Car Detected by Sensor 2: ");
    Serial.print(frequency2);    // Print the vibration frequency
    Serial.println(" Hz");
  } else {
    Serial.println("No activity detected by Sensor 2");

    frequency2 = 0;
  }

// Checking the frequency for sensor3
  double frequency3 = getVibrationFrequency(adxl355_sensor3);
  if (frequency3 > 1.5) {
    Serial.print("Car Detected by Sensor 3: ");
    Serial.print(frequency3);    // Print the vibration frequency
    Serial.println(" Hz");
  } else {
    Serial.println("No activity detected by Sensor 3");

    frequency3 = 0;
  }

// Checking the frequency for sensor4
  double frequency4 = getVibrationFrequency(adxl355_sensor4);
  if (frequency4 > 1.5) {
    Serial.print("Car Detected by Sensor 4: ");
    Serial.print(frequency4);    // Print the vibration frequency
    Serial.println(" Hz");
  } else {
    Serial.println("No activity detected by Sensor 4");

    frequency4 = 0;
  }




  // Convert data into valid string variables
  String freq1String = "1:" + String(frequency1);
  String freq2String = "2:" + String(frequency2);
  String freq3String = "3:" + String(frequency3);
  String freq4String = "4:" + String(frequency4);


  String allData[] = {freq1String, freq2String, freq3String, freq4String};

  // Send data to LoraWAN module
//   String dataToSend = "AT+SEND=0,19,Sending from ESP32!";

  for (int i = 0; i < 4; i++) {
    String dataToSend = "AT+SEND=0," + String(allData[i].length()) + "," + allData[i];
    Serial.println(dataToSend);
    lorawanSerial.println(dataToSend);
    delay(50);
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
