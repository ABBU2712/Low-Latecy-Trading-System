# Step 1: Base Image (Ubuntu for C++ & Python)
FROM ubuntu:22.04

# Step 2: Install Required Packages
RUN apt update && apt install -y \
    g++ cmake make libboost-all-dev \
    python3 python3-pip \
    nodejs npm \
    && rm -rf /var/lib/apt/lists/*

# Step 3: Set Working Directory
WORKDIR /app

# Step 4: Copy All Files
COPY . .

# Step 5: Compile C++ Trading Engine
RUN g++ -o sor_engine sor_integration.cpp -lboost_system -lpthread -std=c++17

# Step 6: Install Python Dependencies
RUN pip install -r requirements.txt

# Step 7: Install Frontend Dependencies
RUN cd orderbook-dashboard && npm install && npm run build

# Step 8: Expose Ports for WebSocket & UDP
EXPOSE 9001 9002 3000

# Step 9: Run All Services (C++ Engine, Python WebSockets, React)
CMD ["sh", "-c", "./sor_engine & python3 udp_socket_server.py & cd orderbook-dashboard && npm start"]
