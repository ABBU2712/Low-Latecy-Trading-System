#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

// Structure for Venue Data
struct VenueData {
    std::string venue_name;
    double price;
    double liquidity;
    double latency;
    double score; // Computed score
};

// Normalize function
double normalize(double value, double min, double max) {
    if (max - min == 0) return 0.0;
    return (value - min) / (max - min);
}

// Compute score function
double compute_score(const VenueData& venue, double price_weight, double liquidity_weight, double latency_weight) {
    return (price_weight * venue.price) +
           (liquidity_weight * venue.liquidity) +
           (latency_weight * (1.0 - venue.latency)); // Inverted latency
}

// Main SOR Engine logic
int main() {
    // Example venue data
    std::vector<VenueData> venues = {
        {"Venue A", 100.10, 2000, 5},
        {"Venue B", 100.20, 1500, 10},
        {"Venue C", 100.05, 2500, 3}
    };

    // Normalize data
    double min_price = 100.05, max_price = 100.20;
    double min_liquidity = 1500, max_liquidity = 2500;
    double min_latency = 3, max_latency = 10;

    for (auto& venue : venues) {
        venue.price = normalize(venue.price, min_price, max_price);
        venue.liquidity = normalize(venue.liquidity, min_liquidity, max_liquidity);
        venue.latency = normalize(venue.latency, min_latency, max_latency);
    }

    // Compute scores
    double price_weight = 0.5, liquidity_weight = 0.3, latency_weight = 0.2;
    for (auto& venue : venues) {
        venue.score = compute_score(venue, price_weight, liquidity_weight, latency_weight);
    }

    // Sort venues by score
    std::sort(venues.begin(), venues.end(), [](const VenueData& a, const VenueData& b) {
        return a.score > b.score;
    });

    // Print ranked venues
    std::cout << "Ranked Venues:\n";
    for (const auto& venue : venues) {
        std::cout << venue.venue_name << " - Score: " << std::fixed << std::setprecision(2) << venue.score << "\n";
    }

    // Split orders
    double order_size = 1000; // Example order size
    std::cout << "\nOrder Allocation:\n";
    double total_score = 0.0;
    for (const auto& venue : venues) total_score += venue.score;

    for (const auto& venue : venues) {
        double allocation = (venue.score / total_score) * order_size;
        std::cout << venue.venue_name << ": " << allocation << " shares\n";
    }

    return 0;
}
