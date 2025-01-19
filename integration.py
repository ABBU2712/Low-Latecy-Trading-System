import queue
import threading
import websocket_client

# Shared queue for market data
market_data_queue = queue.Queue()

# WebSocket client thread
def websocket_client_thread():
    ws = websocket.WebSocketApp(
        "wss://stream.binance.us:9443/ws",
        on_message=lambda ws, msg: market_data_queue.put(json.loads(msg)),
        on_error=lambda ws, err: print("Error:", err),
        on_close=lambda ws, code, msg: print("Connection closed."),
    )
    ws.on_open = lambda ws: ws.send(json.dumps(subscribe_payload))
    ws.run_forever()

# Market Data Listener thread
def market_data_listener():
    while True:
        data = market_data_queue.get()
        if data:
            # Parse and normalize
            top_bid = float(data["bids"][0][0])
            top_ask = float(data["asks"][0][0])
            liquidity = float(data["bids"][0][1]) + float(data["asks"][0][1])
            print(f"Market Data Processed - Price: {(top_bid + top_ask) / 2}, Liquidity: {liquidity}")

# Start threads
threading.Thread(target=websocket_client_thread, daemon=True).start()
market_data_listener()
