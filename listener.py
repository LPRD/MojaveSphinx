import socket
from threading import Thread
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# UDP Configuration
UDP_IP = "192.168.1.2"  # Your computer's IP
UDP_PORT = 8888         # Port to listen on

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

print(f"Listening on {UDP_IP}:{UDP_PORT}")

# Data storage for plotting
time_data = []
lc_data = []
tc_data = []
ox_data = []

# Plot setup
plt.style.use('seaborn')
fig, ax = plt.subplots()
ax.set_title("Real-Time Sensor Data")
ax.set_xlabel("Time (ms)")
ax.set_ylabel("Values")

# Function to update the plot
def update(frame):
    try:
        # Clear and redraw the plot
        ax.clear()
        ax.plot(time_data, lc_data, label="Load Cell (LC)")
        ax.plot(time_data, tc_data, label="TC Pressure")
        ax.plot(time_data, ox_data, label="OX Pressure")
        ax.legend()
        ax.set_title("Real-Time Sensor Data")
        ax.set_xlabel("Time (ms)")
        ax.set_ylabel("Values")
    except Exception as e:
        print(f"Error: {e}")

def read_network_data():
    while True:
        # Receive data
        data, addr = sock.recvfrom(1024)
        message = data.decode()
        print(f"Received message: {message} from {addr}")

        # Parse the data
        parts = message.split(",")
        time = float(parts[0])
        lc = float(parts[1])
        tc = float(parts[2])
        ox = float(parts[3])

        # Append to data lists
        time_data.append(time)
        lc_data.append(lc)
        tc_data.append(tc)
        ox_data.append(ox)

        # Keep only the last 100 data points for better performance
        if len(time_data) > 100:
            time_data.pop(0)
            lc_data.pop(0)
            tc_data.pop(0)
            ox_data.pop(0)

Thread(target=read_network_data).start()

# Animate the plot
ani = FuncAnimation(fig, update, interval=100)  # Update every 100ms
plt.show()