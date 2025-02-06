#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP

#include <vector>
#include <algorithm>
#include <iostream>

struct OrderEntry {
    double price;
    double amount;
};

class OrderBook {
private:
    std::vector<OrderEntry> bids;
    std::vector<OrderEntry> asks;

public:
    void update(const std::vector<OrderEntry>& new_bids, const std::vector<OrderEntry>& new_asks) {
        bids = new_bids;
        asks = new_asks;
        sort_order_book();
    }

    void sort_order_book() {
        std::sort(bids.begin(), bids.end(), [](const OrderEntry& a, const OrderEntry& b) {
            return a.price > b.price; // Highest bid first
        });

        std::sort(asks.begin(), asks.end(), [](const OrderEntry& a, const OrderEntry& b) {
            return a.price < b.price; // Lowest ask first
        });
    }

    OrderEntry get_best_bid() const {
        return bids.empty() ? OrderEntry{0.0, 0.0} : bids.front();
    }

    OrderEntry get_best_ask() const {
        return asks.empty() ? OrderEntry{0.0, 0.0} : asks.front();
    }

    const std::vector<OrderEntry>& get_bids() const { return bids; }
    const std::vector<OrderEntry>& get_asks() const { return asks; }

    // ✅ Reduce the bid after a trade execution
    void reduce_bid(double price, double amount) {
        for (auto& bid : bids) {
            if (bid.price == price) {
                bid.amount -= amount;
                if (bid.amount <= 0) { // Remove bid if fully consumed
                    bids.erase(std::remove_if(bids.begin(), bids.end(), [](OrderEntry& b) { return b.amount <= 0; }), bids.end());
                }
                break;
            }
        }
    }

    // ✅ Reduce the ask after a trade execution
    void reduce_ask(double price, double amount) {
        for (auto& ask : asks) {
            if (ask.price == price) {
                ask.amount -= amount;
                if (ask.amount <= 0) { // Remove ask if fully consumed
                    asks.erase(std::remove_if(asks.begin(), asks.end(), [](OrderEntry& a) { return a.amount <= 0; }), asks.end());
                }
                break;
            }
        }
    }
};

#endif // ORDER_BOOK_HPP
