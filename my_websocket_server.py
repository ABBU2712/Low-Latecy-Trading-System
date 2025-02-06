from flask import Flask
from flask_socketio import SocketIO, emit
import json

app = Flask(__name__)
socketio = SocketIO(app)

@app.route('/')
def index():
    return "WebSocket server is running!"

@socketio.on('get_data')
def handle_get_data():
    # Send sample bids and asks
    data = {
        "bids": [["103.45", "0.2"], ["103.50", "0.1"]],
        "asks": [["104.00", "0.3"], ["104.10", "0.2"]]
    }
    emit('data', json.dumps(data))  # Send data to the client
    print(data)

if __name__ == '__main__':
    socketio.run(app, host='127.0.0.1', port=5000)
