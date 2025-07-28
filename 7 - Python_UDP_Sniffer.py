# Read the UDP stream received by this computer when the IP and Port are known

import socket

# Configuration
UDP_IP = "192.168.1.34"       # Listen on all available interfaces
UDP_PORT = 14550         # Port to listen on (change if needed)
BUFFER_SIZE = 1024       # Size of the receive buffer

# Create UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

print(f"Listening for UDP packets on {UDP_IP}:{UDP_PORT}...")

try:
    while True:
        data, addr = sock.recvfrom(BUFFER_SIZE)  # Receive packet
        print(f"Received {len(data)} bytes from {addr}: {data.hex()}")
except KeyboardInterrupt:
    print("UDP listener stopped.")
finally:
    sock.close()
