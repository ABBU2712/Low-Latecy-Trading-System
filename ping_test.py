import requests
import time

start_time = time.time()
response = requests.get("https://www.coinbase.com/en-gb")
end_time = time.time()

latency = (end_time - start_time) * 1000  # Convert to milliseconds
print(f"API Latency to Venue A: {latency:.2f} ms")
