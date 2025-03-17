# 🚀 Low-Latency Trading Dashboard
**Real-time WebSocket & UDP-based Order Book for High-Frequency Trading (HFT)**  
**Built using C++ (Boost Asio, WebSocket++, UDP) & React.js**  
**Simulates trading, order execution, and trade metrics in real-time**

<img width="1266" alt="Low Latency Trading Dashboard" src="https://github.com/user-attachments/assets/96e613f6-aa5b-40e1-9d93-8522c5118709" />

---

## 📌 Features
- ✅ **Real-time Order Book** – Live bid/ask prices streamed via WebSocket & UDP  
- ✅ **Trade Execution** – Simulates trade matching with configurable latency  
- ✅ **Metrics Dashboard** – Displays latency, trade volume, and price spread in real-time  
- ✅ **Trade Simulator** – Users can simulate buying/selling with market-like latency  
- ✅ **Optimized for Low Latency** – Leverages UDP messaging for faster data transfer  
- ✅ **Free Deployment Guide** – Easily host on **Railway.app, Fly.io, and Vercel**

---

## 📂 Project Structure

```plaintext
.
├── backend
│   ├── src
│   │   ├── main.cpp          # Server entry point
│   │   ├── order_book.cpp    # Order book simulation logic
│   │   ├── trade_executor.cpp# Handles trade matching and latency simulation
│   │   └── ...               # Other backend modules
│   └── include
│       └── ...               # Header files for backend
├── frontend
│   ├── public                # Static assets
│   └── src
│       ├── App.js            # Main React component
│       └── components
│           ├── OrderBook.js  # Displays real-time order book
│           ├── Metrics.js    # Renders trade metrics dashboard
│           └── Simulator.js  # User interface for trade simulation
├── docs
│   └── deployment_guide.md   # Detailed steps for deploying on Railway, Fly.io, and Vercel
├── tests
│   ├── unit_tests            # Unit tests for individual modules
│   └── integration_tests     # End-to-end testing scripts
├── CMakeLists.txt            # Build configuration for the backend
└── README.md                 # Project documentation (this file)


## ⚙️ Installation Prerequisites
Backend:
C++ compiler
CMake
Boost libraries
WebSocket++ library
Frontend:
Node.js (v12+ recommended)
npm or yarn
