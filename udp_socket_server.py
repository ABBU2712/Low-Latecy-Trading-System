import socket
import json

# Set up the UDP socket
UDP_IP = "127.0.0.1"
UDP_PORT = 5005

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

print(f"Market Data Listener is running on {UDP_IP}:{UDP_PORT}...")

def parse_market_data(data):
    """Parses raw market data into a dictionary."""
    fields = {}
    items = data.split(", ")
    for item in items:
        key, value = item.split("=")
        fields[key.strip()] = float(value)
    return fields

# Listen for data
while True:
    data, addr = sock.recvfrom(1024)
    raw_data = data.decode('utf-8')
    venue, market_info = raw_data.split(": ", 1)
    parsed_data = {"venue": venue.strip(), **parse_market_data(market_info)}
    print(f"Parsed Data: {parsed_data}")
