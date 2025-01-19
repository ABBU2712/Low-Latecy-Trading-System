import socket
import time

# Step 1: Set up the UDP socket
UDP_IP = "127.0.0.1"  # Localhost for testing
UDP_PORT = 5005       # Port to send data

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Step 2: Simulate sending market data
market_data_samples = [
    "Venue A: Price=100.10, Liquidity=2000",
    "Venue B: Price=100.20, Liquidity=1500",
    "Venue C: Price=100.05, Liquidity=2500"
]

for data in market_data_samples:
    sock.sendto(data.encode('utf-8'), (UDP_IP, UDP_PORT))
    print(f"Sent: {data}")
    time.sleep(1)  # Wait for 1 second before sending the next packet
