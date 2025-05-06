import socket
import os
from datetime import datetime

# UDP Configuration
UDP_IP = "192.168.1.2"  # Your computer's IP
UDP_PORT = 8888         # Port to listen on

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

print(f"Listening on {UDP_IP}:{UDP_PORT}")

# Create a new file with a unique name
timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
filename = f"udp_data_{timestamp}.txt"

# Ensure the file is created in the current directory
filepath = os.path.join(os.getcwd(), filename)

print(f"Writing data to {filepath}")

def read_network_data():
    with open(filepath, "w") as file:
        while True:
            # Receive data
            data, addr = sock.recvfrom(1024)
            message = data.decode()

            # Parse the data
            parts = message.split(",")
            time = float(parts[0])
            lc = float(parts[1])
            tc = float(parts[2])
            ox = float(parts[3])

            # Write the data to the file
            file.write(f"{time}, {lc}, {tc}, {ox}\n")
            file.flush()  # Ensure data is written to the file immediately

            # Print the data to the console
            print(f"{time} Load Cell: {lc} Thrust Chamber: {tc} Oxidizer: {ox}")

read_network_data()