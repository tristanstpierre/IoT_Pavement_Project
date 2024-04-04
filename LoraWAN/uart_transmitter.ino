#include <HardwareSerial.h>

// Define LoRaWAN module UART pins
#define LORAWAN_RX_PIN 16
#define LORAWAN_TX_PIN 17

// Define LoRaWAN module baud rate
#define LORAWAN_BAUD_RATE 115200

HardwareSerial lorawanSerial(1);

void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);
  while (!Serial);

  // Initialize LoRaWAN module Serial
  lorawanSerial.begin(LORAWAN_BAUD_RATE, SERIAL_8N1, LORAWAN_RX_PIN, LORAWAN_TX_PIN);

  lorawanSerial.println("AT");
  // Serial.print("Received: ");
  
  lorawanSerial.println("AT+RESET");
  // Serial.print("Received: ");


  
}

void loop() {
  // Check if data is available from LoRaWAN module

  /*
  if (lorawanSerial.available()) {
    // Read data from LoRaWAN module
    char receivedChar = lorawanSerial.read();
    // Do something with the received data
    // Serial.print("Received: ");
    Serial.print(receivedChar);
  }

  */

  // Example sending data to LoRaWAN module
  String dataToSend = "AT+SEND=0,19,Sending from ESP32!";
  Serial.println(dataToSend);
  lorawanSerial.println(dataToSend);

  // Wait before sending the next data
  delay(500);
}
