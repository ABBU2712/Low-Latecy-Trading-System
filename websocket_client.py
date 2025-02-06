import asyncio
import websockets
import json

async def send_metrics(websocket, path=None):
    while True:
        try:
            with open("metrics.json", "r") as file:
                data = json.load(file)
                await websocket.send(json.dumps(data))
                print(f"Sent: {json.dumps(data, indent=4)}")
        except Exception as e:
            print(f"Error reading file: {e}")

        await asyncio.sleep(5)  # Send data every 5 seconds

async def main():
    async with websockets.serve(send_metrics, "127.0.0.1", 9001):
        print("WebSocket Server Started on ws://127.0.0.1:9001/")
        await asyncio.Future()  # Keeps the server running

if __name__ == "__main__":
    asyncio.run(main())  # ✅ This ensures the event loop runs correctly
