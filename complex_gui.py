# Author: Phillip Jones
# Date: 08/19/2022, updated 05/25/2023, updated 07/13/2023
# Description: Simple client for sending/recieveing information using a UART interface, or Network Socket
#              to either a mock CyBot server, or the actual CyBot
#              (edited: driving buttons, key presses, Scan button with embedded polar plot)
from tkinter import *
import tkinter as tk
# General Python tutorials (W3schools):  https://www.w3schools.com/python/

# Serial library:  https://pyserial.readthedocs.io/en/latest/shortintro.html 
# import serial  # If using UART option (i.e., using a physical cable), then make sure serial library has been installed, and uncomment this line.
import time # Time library   
# Socket library:  https://realpython.com/python-sockets/  
# See: Background, Socket API Overview, and TCP Sockets  
import socket   
import threading
import os

import numpy as np
import matplotlib
matplotlib.use("TkAgg")
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

# Choose to create either a UART or TCP port socket to communicate with Cybot (Not both!!: I.e, comment out the one not being used)
# UART BEGIN
#cybot = serial.Serial('COM100', 115200)  # UART (Make sure you are using the correct COM port and Baud rate!!)
# UART END

# TCP Socket BEGIN (See Echo Client example): https://realpython.com/python-sockets/#echo-client-and-server
HOST = "192.168.1.1"  # The server's hostname or IP address
PORT = 288        # The port used by the server
# For the mock server instead use:  HOST = "127.0.0.1", PORT = 65432
cybot_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # Create a socket object
cybot_socket.connect((HOST, PORT))   # Connect to the socket  (Note: Server must first be running)
                      
cybot = cybot_socket.makefile("rbw", buffering=0)  # makefile creates a file object out of a socket:  https://pythontic.com/modules/socket/makefile
# TCP Socket END

distance_data=[]
scan_angles = []
scan_distances = []
scanning = False   # True while a scan is being received (blocks other commands)

DATA_FILE = os.path.join(os.path.dirname(os.path.abspath(__file__)), "sensor-scan.txt")


def send(cmd):
    if scanning:        # don't mix drive commands into scan data
        return
    cybot.write((cmd + '\n').encode())
    print("Sent command: " + cmd)
    status_label.config(text="Last Command Sent: " + cmd)

def to_quit():
    cybot.write(("q" + '\n').encode())
    time.sleep(2) # Sleep for 2 seconds
    cybot.close()         # Close file object associated with the socket or UART
    cybot_socket.close()  # Close the socket (NOTE: comment out if using UART interface, only use for network socket option)
    window.destroy()

def scan():
    # Button handler: start the scan in a thread so the GUI doesn't freeze
    if scanning:
        return
    threading.Thread(target=scan_worker, daemon=True).start()

def scan_worker():
    global scanning, scan_angles, scan_distances
    scanning = True
    window.after(0, lambda: status_label.config(text="Last Command Sent: m (scanning...)"))
    cybot.write(("m" + '\n').encode())
    print("Sent command: m")
    distance_data.clear()
    angles_tmp = []
    distances_tmp = []

    with open(DATA_FILE, 'w') as f:
        f.write("angle_deg  distance\n")
        while True:
            try:
                line = cybot.readline().decode(errors='replace').strip()
            except Exception:
                break
            if line() == "END":      # make sure to make Cybot send END
                break
            if not line:
                continue
            distance_data.append(line)
            f.write(line + "\n")
            print("[scan] " + line)
            parts = line.split()
            if len(parts) >= 2:
                try:
                    angles_tmp.append(float(parts[0]))
                    distances_tmp.append(float(parts[1]))
                except ValueError:
                    pass    # skip text/header lines

    scan_angles = angles_tmp
    scan_distances = distances_tmp
    scanning = False
    print("Scan complete: %d points" % len(angles_tmp))
    window.after(0, update_polar_plot)

def update_polar_plot():
    status_label.config(text="Last Command Sent: m (scan done, %d points)" % len(scan_angles))
    if not scan_angles:
        return
    ax.clear()
    ax.plot(np.radians(scan_angles), scan_distances, color='r', linewidth=3.0)
    ax.set_thetamin(0)
    ax.set_thetamax(180)
    ax.set_rmax(max(scan_distances) * 1.1 if max(scan_distances) > 0 else 1)
    ax.set_xticks(np.arange(0, np.pi + .1, np.pi/4))
    ax.set_rlabel_position(-22.5)
    ax.grid(True)
    ax.set_title("CyBot Sensor Scan (0 to 180 degrees)", size=12, y=1.0, pad=-24)
    plot_canvas.draw()


window = Tk()
window.title("cybot communicate")

button_1 = Button(window, text="Forward", width=20, command=lambda: send("w"))
button_2 = Button(window, text="Left", width=20, command=lambda: send("a"))
button_3 = Button(window, text="Right", width=20, command=lambda: send("d"))
button_4 = Button(window, text="Backward", width=20, command=lambda: send("s"))
button_5 = Button(window, text="Quit", width=20, command=to_quit)
button_6 = Button(window, text="Scan", width=20, command=scan)

button_1.grid(row=0, column=1, pady=5)
button_2.grid(row=1, column=0, padx=5)
button_3.grid(row=1, column=2, padx=5)
button_4.grid(row=2, column=1, pady=5)
button_5.grid(row=3, column=1, pady=10)
button_6.grid(row=4, column=1, pady=5)

status_label = Label(window, text="Last Command Sent: ")
status_label.grid(row=5, column=0, columnspan=3, pady=5)

# Key presses also drive the CyBot
window.bind("<w>", lambda e: send("w"))
window.bind("<a>", lambda e: send("a"))
window.bind("<s>", lambda e: send("s"))
window.bind("<d>", lambda e: send("d"))
window.bind("<Up>", lambda e: send("w"))
window.bind("<Left>", lambda e: send("a"))
window.bind("<Down>", lambda e: send("s"))
window.bind("<Right>", lambda e: send("d"))
window.bind("<m>", lambda e: scan())
window.bind("<q>", lambda e: to_quit())

# Embedded polar plot (to the right of the buttons)
plot_frame = tk.Frame(window, padx=8, pady=8)
plot_frame.grid(row=0, column=3, rowspan=6, sticky="nsew")

fig, ax = plt.subplots(subplot_kw={"projection": "polar"}, figsize=(5.2, 4.2))
ax.set_title("CyBot Sensor Scan\n(press Scan to populate)", size=10, y=1.0, pad=-20)
ax.set_thetamax(180)
ax.set_xticks(np.arange(0, np.pi + .1, np.pi/4))
ax.grid(True)

plot_canvas = FigureCanvasTkAgg(fig, master=plot_frame)
plot_canvas.draw()
plot_canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

window.mainloop()
