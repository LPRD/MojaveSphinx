import socket

# UDP Configuration
UDP_IP = "192.168.1.2"  # Your computer's IP
UDP_PORT = 8888         # Port to listen on

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

print(f"Listening on {UDP_IP}:{UDP_PORT}")


def read_network_data():
    while True:
        # Receive data
        data, addr = sock.recvfrom(1024)
        message = data.decode()
        # print(message)

        # Parse the data
        parts = message.split(",")
        time = float(parts[0])
        lc = float(parts[1])
        tc = float(parts[2])
        ox = float(parts[3])

        print(f"{time} Load Cell: {lc} Thrust Chamber: {tc} Oxidizer: {ox}")

read_network_data()