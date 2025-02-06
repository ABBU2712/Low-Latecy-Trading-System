import asyncio
import websockets
import json

BINANCE_WS_URL = "wss://stream.binance.us:9443/ws/btcusdt@depth"

async def binance_handler(websocket, path=None):
    async with websockets.connect(BINANCE_WS_URL) as binance_ws:
        while True:
            try:
                message = await ws.recv()  # Receive Binance order book update
                data = json.loads(message)  # Parse JSON

                # Extract bids and asks
                bids = [(float(price), float(amount)) for price, amount in data.get("b", [])]
                asks = [(float(price), float(amount)) for price, amount in data.get("a", [])]

                print("\n📊 Live Binance Order Book Update:")
                print(f"Bids: {bids}")
                print(f"Asks: {asks}")
            except Exception as e:
                print(f"Error: {e}")
                break

async def start_server():
    server = await websockets.serve(binance_handler, "127.0.0.1", 5000)
    await server.wait_closed()

if __name__ == "__main__":
    print("Starting WebSocket server for Binance data...")
    asyncio.run(start_server())
