#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iomanip>

// Structure for venue data
struct VenueData {
    std::string venue_name;
    double price;
    double liquidity;
    double latency;
    double score;
};

// Structure for execution metrics
struct ExecutionMetrics {
    double fill_rate;
    double slippage;
    double latency;
};

// Normalize data
double normalize(double value, double min, double max) {
    if (max - min == 0) return 0.0; // Avoid division by zero
    return (value - min) / (max - min);
}

// Compute venue score
double compute_score(const VenueData& venue, const std::map<std::string, double>& weights) {
    return (weights.at("price") * venue.price) +
           (weights.at("liquidity") * venue.liquidity) +
           (weights.at("latency") * (1.0 - venue.latency));
}

// Calculate metrics
ExecutionMetrics calculate_metrics(double executed_volume, double order_volume, double execution_price, double expected_price, double send_time, double receive_time) {
    ExecutionMetrics metrics;
    metrics.fill_rate = (executed_volume / order_volume) * 100;
    metrics.slippage = ((execution_price - expected_price) / expected_price) * 100;
    metrics.latency = (receive_time - send_time) * 1000; // Convert seconds to ms
    return metrics;
}

// Adjust weights dynamically
void adjust_weights(std::map<std::string, double>& weights, const ExecutionMetrics& metrics) {
    double learning_rate = 0.1;

    weights["price"] += learning_rate * (1.0 - metrics.slippage);
    weights["liquidity"] += learning_rate * (metrics.fill_rate / 100.0);
    weights["latency"] -= learning_rate * (metrics.latency / 1000.0);

    // Normalize weights
    double total_weight = weights["price"] + weights["liquidity"] + weights["latency"];
    weights["price"] /= total_weight;
    weights["liquidity"] /= total_weight;
    weights["latency"] /= total_weight;
}

// Main function
int main() {
    // Initialize weights
    std::map<std::string, double> weights = {{"price", 0.5}, {"liquidity", 0.3}, {"latency", 0.2}};

    // Sample venue data
    std::vector<VenueData> venues = {
        {"Venue A", 0.33, 0.50, 0.29},
        {"Venue B", 1.00, 0.00, 1.00},
        {"Venue C", 0.00, 1.00, 0.00}
    };

    // Compute initial scores
    for (auto& venue : venues) {
        venue.score = compute_score(venue, weights);
    }

    // Sort venues by score
    std::sort(venues.begin(), venues.end(), [](const VenueData& a, const VenueData& b) {
        return a.score > b.score;
    });

    // Simulated order execution
    std::vector<std::map<std::string, double>> executions = {
        {{"order_volume", 1000}, {"executed_volume", 800}, {"expected_price", 100.0}, {"execution_price", 100.5}, {"send_time", 0.001}, {"receive_time", 0.0035}},
        {{"order_volume", 500}, {"executed_volume", 500}, {"expected_price", 101.0}, {"execution_price", 100.8}, {"send_time", 0.002}, {"receive_time", 0.004}},
    };

    // Process each execution
    for (size_t i = 0; i < executions.size(); ++i) {
        auto exec = executions[i];

        // Calculate metrics
        ExecutionMetrics metrics = calculate_metrics(
            exec["executed_volume"], exec["order_volume"],
            exec["execution_price"], exec["expected_price"],
            exec["send_time"], exec["receive_time"]
        );

        // Adjust weights based on metrics
        adjust_weights(weights, metrics);

        // Recompute scores with updated weights
        for (auto& venue : venues) {
            venue.score = compute_score(venue, weights);
        }

        // Output updated weights
        std::cout << "Updated Weights after Execution " << (i + 1) << ":\n";
        std::cout << "  Price: " << std::fixed << std::setprecision(2) << weights["price"] << "\n";
        std::cout << "  Liquidity: " << weights["liquidity"] << "\n";
        std::cout << "  Latency: " << weights["latency"] << "\n\n";
    }

    return 0;
}
