# For UART communication
import serial
import time

# For graphing 
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Configure the serial port
ser = serial.Serial(
    port='/dev/ttyS0',  # Adjust this to match your UART port (e.g., /dev/ttyS0 for Raspberry Pi 4)
    baudrate=115200,    # Baud rate for communication with RYLR896 module
    timeout=1           # Set timeout for serial communication
)

# Sending to LoraWan
def send_command(command):
    ser.write(command.encode('utf-8'))
    time.sleep(0.1)
    response = ser.readline().decode('utf-8').strip()
    return response


# Reading from LoraWan
def receive_data():
    while True:
        if ser.in_waiting > 0:

            received_data = ser.readline().decode('utf-8').strip()
            print(received_data)

            x = received_data.split(":")[0].split(",")[2]
            y = received_data.split(":")[1].split(",")[0]

            result_tuple = (int(number), float(value))

            ani = animation.FuncAnimation(fig, animate, interval=1000)
            plt.show()



# Animated graph
def animate(i):
    # pullData = open("sampleText.txt","r").read()
    # dataArray = pullData.split('\n')
    xar = []
    yar = []
    xar.append(x)
    yar.append(y)
    ax1.clear()
    ax1.plot(xar,yar)






# Main method
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

        # Open the graph 
        fig = plt.figure()
        ax1 = fig.add_subplot(1,1,1)


        # Received data
        receive_data()
        
        

    except serial.SerialException as e:
        print("Serial port error:", e)

    finally:
        ser.close()

if __name__ == "__main__":
    main()

