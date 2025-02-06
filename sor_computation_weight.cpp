#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>

// Structure for execution metrics
struct ExecutionMetrics {
    double fill_rate;   // Percentage of order filled
    double slippage;    // Difference between expected and actual execution price
    double latency;     // Time taken to complete the order in ms
    std::string venue;  // Venue name
};

// Function to calculate metrics
ExecutionMetrics calculate_metrics(double executed_volume, double order_volume, double execution_price, double expected_price, double send_time, double receive_time, std::string venue) {
    ExecutionMetrics metrics;
    metrics.fill_rate = (executed_volume / order_volume) * 100; // Fill rate percentage
    metrics.slippage = ((execution_price - expected_price) / expected_price) * 100; // Slippage percentage
    metrics.latency = (receive_time - send_time) * 1000; // Convert seconds to milliseconds
    metrics.venue = venue;
    return metrics;
}

// Function to adjust weights dynamically
void adjust_weights(std::map<std::string, double>& weights, const ExecutionMetrics& metrics) {
    double learning_rate = 0.1; // Adjustment rate

    // Adjust weights based on metrics
    weights["price"] += learning_rate * (1.0 - metrics.slippage);
    weights["liquidity"] += learning_rate * (metrics.fill_rate / 100.0);
    weights["latency"] -= learning_rate * (metrics.latency / 1000.0);

    // Normalize weights to ensure they sum to 1
    double total_weight = weights["price"] + weights["liquidity"] + weights["latency"];
    weights["price"] /= total_weight;
    weights["liquidity"] /= total_weight;
    weights["latency"] /= total_weight;
}

// Main function to test the workflow
int main() {
    // Initialize weights
    std::map<std::string, double> weights = {{"price", 0.5}, {"liquidity", 0.3}, {"latency", 0.2}};

    // Simulated execution data
    std::vector<std::map<std::string, double>> executions = {
        {{"order_volume", 1000}, {"executed_volume", 800}, {"expected_price", 100.0}, {"execution_price", 100.5}, {"send_time", 0.001}, {"receive_time", 0.0035}},
        {{"order_volume", 500}, {"executed_volume", 500}, {"expected_price", 101.0}, {"execution_price", 100.8}, {"send_time", 0.002}, {"receive_time", 0.004}},
        {{"order_volume", 700}, {"executed_volume", 600}, {"expected_price", 102.0}, {"execution_price", 102.5}, {"send_time", 0.0015}, {"receive_time", 0.003}}
    };

    // Process each execution
    for (size_t i = 0; i < executions.size(); ++i) {
        auto exec = executions[i];

        // Calculate metrics
        ExecutionMetrics metrics = calculate_metrics(
            exec["executed_volume"], exec["order_volume"],
            exec["execution_price"], exec["expected_price"],
            exec["send_time"], exec["receive_time"], "Venue A"
        );

        // Output metrics
        std::cout << "Execution " << (i + 1) << " Metrics:\n";
        std::cout << "  Fill Rate: " << metrics.fill_rate << "%\n";
        std::cout << "  Slippage: " << metrics.slippage << "%\n";
        std::cout << "  Latency: " << metrics.latency << " ms\n";

        // Adjust weights based on metrics
        adjust_weights(weights, metrics);

        // Output updated weights
        std::cout << "Updated Weights:\n";
        std::cout << "  Price Weight: " << std::fixed << std::setprecision(2) << weights["price"] << "\n";
        std::cout << "  Liquidity Weight: " << weights["liquidity"] << "\n";
        std::cout << "  Latency Weight: " << weights["latency"] << "\n\n";
    }

    return 0;
}
