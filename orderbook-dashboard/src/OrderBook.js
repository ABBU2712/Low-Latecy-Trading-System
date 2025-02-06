import React, { useState, useEffect } from "react";
import "./OrderBook.css";

function OrderBook() {
  const [orderBook, setOrderBook] = useState({ bids: [], asks: [] });
  const [prevOrderBook, setPrevOrderBook] = useState({ bids: [], asks: [] });

  useEffect(() => {
    const ws = new WebSocket("ws://127.0.0.1:9001/");
    
    ws.onmessage = (event) => {
      const data = JSON.parse(event.data);
      if (data.order_book) {
        setOrderBook((prevOrderBook) => {
          const updatedBids = [...data.order_book.bids, ...prevOrderBook.bids].slice(0, 10);
          const updatedAsks = [...data.order_book.asks, ...prevOrderBook.asks].slice(0, 10);
          return { bids: updatedBids, asks: updatedAsks };
        });
        setPrevOrderBook(orderBook);
      }
    };

    ws.onclose = () => {
      console.error("WebSocket disconnected.");
    };

    return () => ws.close();
  }, [orderBook]);

  const getPriceChangeIndicator = (prevPrice, currentPrice) => {
    if (prevPrice === undefined) return "";
    return currentPrice > prevPrice ? " 🔼" : currentPrice < prevPrice ? " 🔽" : "";
  };

  return (
    <div className="order-book-container">
      <h1>📊 Real-Time Order Book</h1>
      <div className="order-book">
        <div className="order-column">
          <h2>🟢 Bids</h2>
          <ul>
            {orderBook.bids.map((bid, index) => (
              <li key={index}>
                {bid?.stock || "N/A"} | {bid?.price ? bid.price.toFixed(2) : "-"}
                {getPriceChangeIndicator(prevOrderBook.bids[index]?.price, bid?.price)} |
                {bid?.amount ? bid.amount.toFixed(5) : "-"}
              </li>
            ))}
          </ul>
        </div>
        <div className="order-column">
          <h2>🔴 Asks</h2>
          <ul>
            {orderBook.asks.map((ask, index) => (
              <li key={index}>
                {ask?.stock || "N/A"} | {ask?.price ? ask.price.toFixed(2) : "-"}
                {getPriceChangeIndicator(prevOrderBook.asks[index]?.price, ask?.price)} |
                {ask?.amount ? ask.amount.toFixed(5) : "-"}
              </li>
            ))}
          </ul>
        </div>
      </div>
    </div>
  );
}

export default OrderBook;
