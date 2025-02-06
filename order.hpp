#ifndef ORDER_HPP
#define ORDER_HPP

#include <string>

struct Order {
    int order_id;
    std::string side;    // "BUY" or "SELL"
    double price;
    double quantity;
    std::string order_type; // "MARKET" or "LIMIT"

    Order(int id, std::string s, double p, double q, std::string type) 
        : order_id(id), side(s), price(p), quantity(q), order_type(type) {}
};

#endif // ORDER_HPP
