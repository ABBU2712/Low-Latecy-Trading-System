import React, { useState, useEffect } from "react";
import "./TradeSimulator.css";

function TradeSimulator() {
  const [webSocket, setWebSocket] = useState(null);
  const [error, setError] = useState(null);
  const [stock, setStock] = useState("BTC/USD");
  const [amount, setAmount] = useState("");
  const [statusMessage, setStatusMessage] = useState("");

  useEffect(() => {
    function connectWebSocket() {
      const ws = new WebSocket("ws://127.0.0.1:9001/");

      ws.onopen = () => {
        console.log("✅ WebSocket connected.");
        setError(null);
        setWebSocket(ws);
      };

      ws.onerror = (err) => {
        console.error("WebSocket Error:", err);
        setError("⚠ WebSocket connection error.");
      };

      ws.onclose = () => {
        console.warn("⚠ WebSocket disconnected. Retrying in 5 seconds...");
        setError("⚠ WebSocket is not connected.");
        setTimeout(connectWebSocket, 5000); // Auto-reconnect
      };

      ws.onmessage = (event) => {
        try {
          const data = JSON.parse(event.data);
          console.log("📩 Received WebSocket Message:", data);
          if (data.trade_status) {
            setStatusMessage(data.trade_status);
          }
        } catch (err) {
          console.error("WebSocket Message Parsing Error:", err);
        }
      };
    }

    connectWebSocket();

    return () => {
      if (webSocket) {
        webSocket.close();
      }
    };
  }, []);

  const executeTrade = () => {
    if (!webSocket || webSocket.readyState !== WebSocket.OPEN) {
      setError("⚠ WebSocket is not connected. Please wait...");
      return;
    }

    const tradeRequest = {
      action: "execute_trade",
      stock: stock,
      amount: parseFloat(amount),
    };

    console.log("📤 Sending Trade Request:", tradeRequest);
    webSocket.send(JSON.stringify(tradeRequest));
    setStatusMessage("Trade request sent...");
  };

  return (
    <div className="trade-simulator">
      <h1>Trade Simulator</h1>
      {error && <p className="error-message">{error}</p>}
      <label>Stock:</label>
      <select value={stock} onChange={(e) => setStock(e.target.value)}>
        <option value="BTC/USD">BTC/USD</option>
        <option value="ETH/USD">ETH/USD</option>
      </select>

      <label>Amount:</label>
      <input
        type="number"
        value={amount}
        onChange={(e) => setAmount(e.target.value)}
        placeholder="Enter trade amount"
      />

      <button onClick={executeTrade}>Execute Trade</button>

      {statusMessage && <p className="trade-status">{statusMessage}</p>}
    </div>
  );
}

export default TradeSimulator;
