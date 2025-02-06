import React, { useEffect, useState } from "react";
import { LineChart, Line, XAxis, YAxis, Tooltip, Legend, ResponsiveContainer } from "recharts";

const MetricsChart = () => {
    const [metrics, setMetrics] = useState([]);

    useEffect(() => {
        const ws = new WebSocket("ws://127.0.0.1:9001/");
        ws.onmessage = (event) => {
            try {
                const data = JSON.parse(event.data);
                setMetrics((prevMetrics) => [...prevMetrics, {
                    timestamp: new Date().toLocaleTimeString(),
                    trade_volume: data.metrics.trade_volume,
                    price_spread: data.metrics.price_spread,
                    latency: data.metrics.avg_latency_ms
                }]);
            } catch (error) {
                console.error("Error parsing WebSocket message:", error);
            }
        };

        return () => ws.close();
    }, []);

    return (
        <div>
            <h2>📈 Trade Volume vs Latency vs Price Spread</h2>
            <ResponsiveContainer width="100%" height={400}>
                <LineChart data={metrics}>
                    <XAxis dataKey="timestamp" />
                    <YAxis />
                    <Tooltip />
                    <Legend />
                    <Line type="monotone" dataKey="trade_volume" stroke="blue" />
                    <Line type="monotone" dataKey="latency" stroke="red" />
                    <Line type="monotone" dataKey="price_spread" stroke="green" />
                </LineChart>
            </ResponsiveContainer>

        </div>
    );
};

export default MetricsChart;
