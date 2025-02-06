import React from "react";
import styled from "styled-components";
import OrderBook from "./OrderBook";
import MetricsChart from "./MetricsDisplay";
import TradeSimulator from "./TradeSimulator";

const Dashboard = styled.div`
    display: grid;
    grid-template-columns: 1fr 1fr;
    grid-template-rows: 1fr 1fr;
    height: 100vh;
    gap: 15px;
    padding: 15px;
    background-color: #121212;
`;

const Quadrant = styled.div`
    background: #1e1e1e;
    padding: 15px;
    border-radius: 10px;
    box-shadow: 0px 4px 10px rgba(255, 255, 255, 0.1);
`;

function App() {
    return (
        <Dashboard>
            <Quadrant> <OrderBook /> </Quadrant>       {/* 🔹 Order Book (Top Left) */}
            <Quadrant> <MetricsChart /> </Quadrant>    {/* 🔹 Graphs (Top Right) */}
            <Quadrant style={{ gridColumn: "span 2" }}> <TradeSimulator /> </Quadrant> {/* 🔹 Simulator (Bottom) */}
        </Dashboard>
    );
}

export default App;
