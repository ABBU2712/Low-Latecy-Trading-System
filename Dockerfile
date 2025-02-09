# Use Ubuntu as base image
FROM ubuntu:latest

# Install required dependencies
RUN apt-get update && apt-get install -y \
    git cmake g++ libboost-all-dev nlohmann-json3-dev \
    libssl-dev  # ✅ This installs OpenSSL (fixes -lssl & -lcrypto error)

# Set working directory
WORKDIR /app

# Copy project files into the container
COPY . /app

# Install WebSocket++
RUN git clone https://github.com/zaphoyd/websocketpp.git && \
    cp -r websocketpp /usr/include/

# Compile the C++ Trading Engine
RUN g++ -std=c++17 -o sor_engine sor_integration.cpp \
    -I /usr/include/websocketpp -I /usr/include/nlohmann \
    -L /usr/local/lib \
    -lboost_system -lboost_thread -lssl -lcrypto -lpthread

# ✅ Ensure binary is executable
RUN chmod +x /app/sor_engine

# Set entrypoint
CMD ["./sor_engine"]
