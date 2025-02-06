import asyncio
import websockets
import socket
import json

# ✅ UDP Server Configuration
UDP_IP = "127.0.0.1"
UDP_PORT = 9002  # Must match the C++ UDP sender

# ✅ WebSocket Server Configuration
WEBSOCKET_PORT = 9001
connected_clients = set()

# ✅ Create UDP Socket (Listener)
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
udp_socket.bind((UDP_IP, UDP_PORT))

async def handle_websocket(websocket, path=None):
    """Handles WebSocket connections and stores active clients."""
    connected_clients.add(websocket)
    print(f"[WebSocket] New client connected: {websocket.remote_address}")

    try:
        async for _ in websocket:
            pass  # Keep the connection open
    except:
        pass
    finally:
        connected_clients.remove(websocket)
        print(f"[WebSocket] Client disconnected: {websocket.remote_address}")

async def udp_listener():
    """Listens for UDP messages and forwards them to WebSocket clients."""
    print(f"[UDP Listener] Listening on {UDP_IP}:{UDP_PORT}...")
    loop = asyncio.get_event_loop()

    while True:
        # Receive UDP data
        data, _ = await loop.run_in_executor(None, udp_socket.recvfrom, 65536)
        decoded_data = data.decode("utf-8")

        try:
            json_data = json.loads(decoded_data)

            # ✅ Explicitly Create Tasks for WebSocket Sends
            if connected_clients:
                message = json.dumps(json_data)
                tasks = [asyncio.create_task(client.send(message)) for client in connected_clients]
                await asyncio.gather(*tasks)  # ✅ Properly await all sends

            print(f"[UDP Received] Forwarded data to {len(connected_clients)} clients")

        except json.JSONDecodeError:
            print("[ERROR] Invalid JSON received over UDP")

async def main():
    """Runs both the WebSocket server and UDP listener concurrently."""
    websocket_server = websockets.serve(handle_websocket, "127.0.0.1", WEBSOCKET_PORT)
    print(f"[WebSocket] Running on ws://127.0.0.1:{WEBSOCKET_PORT}/")

    await asyncio.gather(websocket_server, udp_listener())

if __name__ == "__main__":
    asyncio.run(main())
