import serial
import time

# Configure the serial port
ser = serial.Serial(
    port='/dev/ttyS0',  # Adjust this to match your UART port (e.g., /dev/ttyS0 for Raspberry Pi 4)
    baudrate=115200,    # Baud rate for communication with RYLR896 module
    timeout=1           # Set timeout for serial communication
)

def send_command(command):
    ser.write(command.encode('utf-8'))
    time.sleep(0.1)
    response = ser.readline().decode('utf-8').strip()
    return response

def receive_data():
    while True:
        if ser.in_waiting > 0:
            received_data = ser.readline().decode('utf-8').strip()
            print(received_data)

def main():
    try:
        ser.flushInput()
        ser.flushOutput()
        
        print("testing connection")
        response = send_command("AT\r\n")
        print("Response:", response)
        
        # Reset the module
        print("Resetting module...")
        response = send_command("AT+RESET\r\n")
        print("Response:", response)
        


        
        """
        # Set LoRaWAN parameters
        print("Setting LoRaWAN parameters...")
        response = send_command("AT+ADDRESS=1\r\n")  # Set address
        print("Response:", response)
        response = send_command("AT+NETWORKID=1234\r\n")  # Set network ID
        print("Response:", response)
        # Continue setting other parameters as needed
        
        # Join the LoRaWAN network
        print("Joining LoRaWAN network...")
        response = send_command("AT+JOIN=OTAA\r\n")
        print("Response:", response)
        
        """

        # Received data
        receive_data()
        
        

    except serial.SerialException as e:
        print("Serial port error:", e)

    finally:
        ser.close()

if __name__ == "__main__":
    main()

