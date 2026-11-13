# Author: Phillip Jones
# Date: 08/19/2022, updated 05/25/2023, updated 07/13/2023
# Description: Simple client for sending/recieveing information using a UART interface, or Network Socket
#              to either a mock CyBot server, or the actual CyBot
from tkinter import *
# General Python tutorials (W3schools):  https://www.w3schools.com/python/

# Serial library:  https://pyserial.readthedocs.io/en/latest/shortintro.html 
# import serial  # If using UART option (i.e., using a physical cable), then make sure serial library has been installed, and uncomment this line.
import time # Time library   
# Socket library:  https://realpython.com/python-sockets/  
# See: Background, Socket API Overview, and TCP Sockets  
import socket   

import subprocess

# Choose to create either a UART or TCP port socket to communicate with Cybot (Not both!!: I.e, comment out the one not being used)
# UART BEGIN
#cybot = serial.Serial('COM100', 115200)  # UART (Make sure you are using the correct COM port and Baud rate!!)
# UART END

# TCP Socket BEGIN (See Echo Client example): https://realpython.com/python-sockets/#echo-client-and-server
HOST = "192.168.1.1"  # The server's hostname or IP address
PORT = 288        # The port used by the server
cybot_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # Create a socket object
cybot_socket.connect((HOST, PORT))   # Connect to the socket  (Note: Server must first be running)
                      
cybot = cybot_socket.makefile("rbw", buffering=0)  # makefile creates a file object out of a socket:  https://pythontic.com/modules/socket/makefile
# TCP Socket END

degrees = []
dist = []

def send(cmd):
    cybot.write((cmd + '\n').encode())
    print("Sent command: " + cmd)
    objectDetect = False
    degrees = []
    dist = []
    while 1:
        rx_message = cybot.readline()
        if "end" in rx_message.decode():
            break

        if "Object" in rx_message.decode():
            objectDetect = True
        
        if not objectDetect and "Degree" not in rx_message.decode():
            split_data = rx_message.decode().strip().split(" ")
            degrees.append(split_data[0])
            dist.append(split_data[-1]) # Not a bug, gets the last element in array
        
        print(rx_message.decode() + "\n") # Convert message from bytes to String (i.e., decode)
        
    if cmd == "m":
        with open("sensor_cleaned.txt", "w") as f:
            f.write("Angle(Degrees)	Distance(m)\n")
            for i in range(len(dist)):
                print(degrees[i] + "    " + dist[i])
                f.write(degrees[i] + "               " + dist[i] + "\n")
            f.close()
        
        subprocess.run(["python", "CyBot-Plot-Sensor-Scan-Values-From-File.py", "sensor_cleaned.txt"])

def to_quit():
    cybot.write(("q" + '\n').encode())
    time.sleep(2) # Sleep for 2 seconds
    cybot.close()         # Close file object associated with the socket or UART
    cybot_socket.close()  # Close the socket (NOTE: comment out if using UART interface, only use for network socket option)
    window.destroy()


window = Tk()
window.title("cybot communicate")

button_1 = Button(window, text="Forward", width=20, command=lambda: send("w"))
button_2 = Button(window, text="Left", width=20, command=lambda: send("a"))
button_3 = Button(window, text="Right", width=20, command=lambda: send("d"))
button_4 = Button(window, text="Backward", width=20, command=lambda: send("s"))
button_5 = Button(window, text="Quit", width=20, command=to_quit)
button_6 = Button(window, text="Toggle", width=20, command=lambda: send("t"))
button_7 = Button(window, text="Auto Scan", width=20, command=lambda: send("h"))
button_8 = Button(window, text="Man Scan", width=20, command=lambda: send("m"))

button_1.grid(row=1, column=1, pady=5)
button_2.grid(row=2, column=0, padx=5)
button_3.grid(row=2, column=2, padx=5)
button_4.grid(row=3, column=1, pady=5)
button_5.grid(row=4, column=1, pady=10)
button_6.grid(row=0, column=1, pady=5)
button_7.grid(row=0, column=2, pady=5)
button_8.grid(row=0, column=0, pady=5)

window.mainloop()

'''
# Send some text: Either 1) Choose "Hello" or 2) have the user enter text to send
send_message = "Hello\n"                            # 1) Hard code message to "Hello", or
send_message = input("Enter a message:") + '\n'   # 2) Have user enter text

cybot.write(send_message.encode()) # Convert String to bytes (i.e., encode), and send data to the server

#print("Sent to server: " + send_message) 

# Send messges to server until user sends "quit"
while send_message != 'quit\n':
        print("wait for server reply\n")
        rx_message = cybot.readline()      # Wait for a message, readline expects message to end with "\n"
        print("Got a message from server: " + rx_message.decode() + "\n") # Convert message from bytes to String (i.e., decode)
        send_message = input("Enter a message (enter quit to exit):") + '\n' # Enter next message to send to server
        cybot.write(send_message.encode()) # Convert String to bytes (i.e., encode), and send data to the server
        
print("Client exiting, and closing file descriptor, and/or network socket\n")
time.sleep(2) # Sleep for 2 seconds
cybot.close()         # Close file object associated with the socket or UART
cybot_socket.close()  # Close the socket (NOTE: comment out if using UART interface, only use for network socket option)
'''
