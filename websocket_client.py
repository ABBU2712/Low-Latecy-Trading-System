import websocket
import json

# Step 1: Define WebSocket URL
socket_url = "wss://stream.binance.us:9443/ws"

subscribe_payload = {
    "method": "SUBSCRIBE",
    "params": [
        "btcusdt@depth5"  # Order book depth for BTC/USDT
    ],
    "id": 1
}

# Step 2: Define WebSocket event handlers
def on_message(ws, message):
    data = json.loads(message)

    # Extract relevant fields from Binance data
    top_bid = float(data["bids"][0][0])  # Best bid price
    top_bid_qty = float(data["bids"][0][1])  # Quantity at best bid
    top_ask = float(data["asks"][0][0])  # Best ask price
    top_ask_qty = float(data["asks"][0][1])  # Quantity at best ask

    # Simulate latency (static placeholder for now)
    latency = 5.0

    # Processed venue data
    venue_data = {
        "venue": "Binance",
        "price": (top_bid + top_ask) / 2,  # Mid-market price
        "liquidity": top_bid_qty + top_ask_qty,  # Combined bid/ask liquidity
        "latency": latency,
    }

    print("Processed Market Data:", venue_data)

def on_error(ws, error):
    print("Error:", error)

def on_close(ws, close_status_code, close_msg):
    print("Connection closed.")

def on_open(ws):
    ws.send(json.dumps(subscribe_payload))
    print("Subscribed to Binance Order Book!")

# Step 3: Start WebSocket client
ws = websocket.WebSocketApp(
    socket_url,
    on_message=on_message,
    on_error=on_error,
    on_close=on_close
)
ws.on_open = on_open
ws.run_forever()
