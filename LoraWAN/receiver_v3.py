import serial
import time

import re
import matplotlib.pyplot as plt

# Configure the serial port
ser = serial.Serial(
    port='/dev/ttyS0',  # Adjust this to match your UART port (e.g., /dev/ttyS0 for Raspberry Pi 4)
    baudrate=115200,    # Baud rate for communication with RYLR896 module
    timeout=1           # Set timeout for serial communication
)

# def send_command(command):
#     ser.write(command.encode('utf-8'))
#     time.sleep(0.1)
#     response = ser.readline().decode('utf-8').strip()
#     return response

def receive_data():
    while True:
        if ser.in_waiting > 0:
            received_data = ser.readline().decode('utf-8').strip()
            print(received_data)
            x_coord, y_coord = parse_input(receive_data)

            if x_coord is not None and y_coord is not None:
                print(x_coord, y_coord)
                x.append(x_coord)
                y.append(y_coord)
                update_graph(x, y, (x_coord, y_coord))
            else:
                print("Invalid input format. Please enter a string in the format: +RCV=<num1>,<num2>,<num3>: <num4>,<num5>,<num6>")




def parse_input(input_str):
    # Use regular expression to extract coordinates from the input string
    match = re.match(r'\+RCV=\d+,\d+,(\d+): ([\d.-]+),[\d.-]+,[\d.-]+', input_str)
    if match:
        x = float(match.group(1))
        y = float(match.group(2))
        return x, y
    else:
        return None, None

def update_graph(x, y, point):
    last_x = x[-15:]  # Take the last 15 x coordinates
    last_y = y[-15:]  # Take the corresponding last 15 y coordinates
    plt.clf()  # Clear the current plot
    plt.scatter(last_x, last_y, color='red')  # Plot the last 15 points
    if point:
        plt.scatter(point[0], point[1], color='blue')  # Plot the new point if available
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title('Graph of Points (Last 15)')
    plt.grid(True)
    plt.draw()  # Update the plot
    plt.pause(0.01)  # Pause for a short while to show the updated plot



def main():


    x = []
    y = []

    plt.ion()  # Turn on interactive mode
    fig = plt.figure()
    ax = fig.add_subplot(111)

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
        
        # Received data
        while True:
            if ser.in_waiting > 0:
                received_data = ser.readline().decode('utf-8').strip()
                print(received_data)
                x_coord, y_coord = parse_input(receive_data)

                if x_coord is not None and y_coord is not None:
                    print(x_coord, y_coord)
                    x.append(x_coord)
                    y.append(y_coord)
                    update_graph(x, y, (x_coord, y_coord))
                else:
                    print("Invalid input format. Please enter a string in the format: +RCV=<num1>,<num2>,<num3>: <num4>,<num5>,<num6>")

        
        plt.ioff()  # Turn off interactive mode
        plt.show()  # Keep the plot window open after exiting the loop
        
        

    except serial.SerialException as e:
        print("Serial port error:", e)

    finally:
        ser.close()

if __name__ == "__main__":
    main()

