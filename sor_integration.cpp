#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <nlohmann/json.hpp>
#include <boost/asio.hpp>
#include <thread>
#include <vector>
#include <set>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include "order_book.hpp"

using json = nlohmann::json;
using ws_server = websocketpp::server<websocketpp::config::asio>;
using udp = boost::asio::ip::udp;

// ✅ Order Book & WebSocket Server
OrderBook order_book;
ws_server server_instance;
std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> connections;

// ✅ UDP Server Configuration
const std::string UDP_CLIENT_IP = "127.0.0.1";  // Update if needed
const int UDP_PORT = 9002;  // UDP Port for broadcasting order book

// ✅ Trade Metrics
double trade_volume = 0.0;
int trade_count = 0;
std::vector<double> latencies;
std::chrono::time_point<std::chrono::high_resolution_clock> last_order_book_update;

// ✅ WebSocket Handlers
void on_open(websocketpp::connection_hdl hdl) {
    connections.insert(hdl);
    std::cout << "[WS] Client connected!\n";
}

void on_close(websocketpp::connection_hdl hdl) {
    connections.erase(hdl);
    std::cout << "[WS] Client disconnected!\n";
}

// ✅ WebSocket Message Handler: Handles Trade Execution Requests
void on_message(websocketpp::connection_hdl hdl, ws_server::message_ptr msg) {
    try {
        json received_data = json::parse(msg->get_payload());
        
        if (received_data["action"] == "execute_trade") {
            std::string stock = received_data["stock"];
            double amount = received_data["amount"];

            auto best_bid = order_book.get_best_bid();
            auto best_ask = order_book.get_best_ask();

            if (best_bid.price > 0 && best_ask.price > 0 && best_bid.price >= best_ask.price) {
                // ✅ Execute Trade
                double trade_price = (best_bid.price + best_ask.price) / 2.0;
                double trade_amount = std::min(best_bid.amount, amount);
                order_book.reduce_bid(best_bid.price, trade_amount);
                order_book.reduce_ask(best_ask.price, trade_amount);

                // ✅ Send Confirmation
                json trade_status = {{"trade_status", "✅ Trade Executed: " + std::to_string(trade_amount) + " at " + std::to_string(trade_price)}};
                server_instance.send(hdl, trade_status.dump(), websocketpp::frame::opcode::text);

                std::cout << "[TRADE] Executed " << trade_amount << " at " << trade_price << std::endl;
            } else {
                json trade_status = {{"trade_status", "⚠ Trade not executed. No matching orders."}};
                server_instance.send(hdl, trade_status.dump(), websocketpp::frame::opcode::text);
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error processing WebSocket message: " << e.what() << std::endl;
    }
}

// ✅ UDP Broadcast Function
void udp_broadcast(boost::asio::io_context& io_context) {
    udp::socket socket(io_context, udp::endpoint(udp::v4(), 0));
    udp::endpoint receiver_endpoint(boost::asio::ip::make_address(UDP_CLIENT_IP), UDP_PORT);

    while (true) {
        json bids_json = json::array();
        json asks_json = json::array();

        for (const auto &bid : order_book.get_bids()) {
            if (bid.amount > 0) {
                bids_json.push_back({{"stock", "BTC/USD"}, {"price", bid.price}, {"amount", bid.amount}});
            }
        }

        for (const auto &ask : order_book.get_asks()) {
            if (ask.amount > 0) {
                asks_json.push_back({{"stock", "BTC/USD"}, {"price", ask.price}, {"amount", ask.amount}});
            }
        }

        double avg_latency = latencies.empty() ? 0.0 : std::accumulate(latencies.begin(), latencies.end(), 0.0) / latencies.size();
        double price_spread = order_book.get_best_ask().price - order_book.get_best_bid().price;

        json payload = {
            {"metrics", {
                {"trade_volume", trade_volume},
                {"trade_count", trade_count},
                {"avg_latency_ms", avg_latency},
                {"price_spread", price_spread}
            }},
            {"order_book", {
                {"bids", bids_json},
                {"asks", asks_json}
            }}
        };

        std::string message = payload.dump();
        socket.send_to(boost::asio::buffer(message), receiver_endpoint);

        std::cout << "[UDP] Sent Order Book + Metrics to UDP Client\n";

        trade_volume = 0.0;
        trade_count = 0;
        latencies.clear();

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

// ✅ Trade Execution Logic (Fixed)
void execute_trade(OrderBook &order_book) {
    auto best_bid = order_book.get_best_bid();
    auto best_ask = order_book.get_best_ask();

    if (best_bid.price > 0 && best_ask.price > 0) {
        if (best_ask.price - best_bid.price <= 20) {  // ✅ Allow trade if spread is small
            double trade_price = (best_bid.price + best_ask.price) / 2.0;
            double trade_amount = std::min(best_bid.amount, best_ask.amount);

            auto now = std::chrono::high_resolution_clock::now();
            double latency_ms = std::chrono::duration<double, std::milli>(now - last_order_book_update).count();
            latencies.push_back(latency_ms);

            trade_volume += trade_amount;
            trade_count++;
            order_book.reduce_bid(best_bid.price, trade_amount);
            order_book.reduce_ask(best_ask.price, trade_amount);
        } else {
            std::cout << "[WARNING] Best Bid < Best Ask - Spread too large, no trade.\n";
        }
    } else {
        std::cout << "[WARNING] No valid bids or asks to execute a trade.\n";
    }
}

// ✅ WebSocket Broadcast
// ✅ WebSocket Broadcast with Debug Logging
void broadcast_combined_data(boost::asio::io_context& io_context) {
    udp::socket socket(io_context, udp::endpoint(udp::v4(), 0));
    udp::endpoint receiver_endpoint(boost::asio::ip::make_address(UDP_CLIENT_IP), UDP_PORT);

    while (true) {
        json bids_json = json::array();
        json asks_json = json::array();

        for (const auto &bid : order_book.get_bids()) {
            if (bid.amount > 0) {
                bids_json.push_back({{"stock", "BTC/USD"}, {"price", bid.price}, {"amount", bid.amount}});
            }
        }

        for (const auto &ask : order_book.get_asks()) {
            if (ask.amount > 0) {
                asks_json.push_back({{"stock", "BTC/USD"}, {"price", ask.price}, {"amount", ask.amount}});
            }
        }

        double avg_latency = latencies.empty() ? 0.0 : std::accumulate(latencies.begin(), latencies.end(), 0.0) / latencies.size();
        double price_spread = order_book.get_best_ask().price - order_book.get_best_bid().price;

        json payload = {
            {"metrics", {
                            {"trade_volume", trade_volume},
                            {"trade_count", trade_count},
                            {"avg_latency_ms", avg_latency},
                            {"price_spread", price_spread}
                        }},
            {"order_book", {
                            {"bids", bids_json},
                            {"asks", asks_json}
                        }}
        };

        std::string message = payload.dump();

        // ✅ Send to WebSocket Clients
        for (auto hdl : connections) {
            server_instance.send(hdl, message, websocketpp::frame::opcode::text);
        }

        // ✅ Send to UDP Clients
        socket.send_to(boost::asio::buffer(message), receiver_endpoint);

        std::cout << "[WS & UDP] Sent Order Book + Metrics to Clients\n";

        trade_volume = 0.0;
        trade_count = 0;
        latencies.clear();

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}


// ✅ WebSocket Server
void run_websocket_server() {
    server_instance.set_reuse_addr(true);
    server_instance.set_listen_backlog(512);
    server_instance.init_asio();
    server_instance.set_open_handler(&on_open);
    server_instance.set_close_handler(&on_close);
    server_instance.set_message_handler(&on_message); 
    server_instance.listen(9001);
    server_instance.start_accept();
    std::cout << "[WS] WebSocket Server running on ws://127.0.0.1:9001/\n";
    server_instance.run();
}

// ✅ Inject Mock Order Book Data
void inject_mock_order_book(OrderBook &order_book) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(2));

        double mock_bid_price = 100000 + (rand() % 500);
        double mock_ask_price = mock_bid_price + (rand() % 50) + 1;  // ✅ Ensure ask > bid
        double mock_amount = (rand() % 10) + 1;

        std::vector<OrderEntry> mock_bids = {{mock_bid_price, mock_amount}};
        std::vector<OrderEntry> mock_asks = {{mock_ask_price, mock_amount}};

        order_book.update(mock_bids, mock_asks);
        last_order_book_update = std::chrono::high_resolution_clock::now();
    }
}

void udp_listener(boost::asio::io_context &io_context) {
    udp::socket socket(io_context, udp::endpoint(udp::v4(), UDP_PORT));
    char buffer[2048];

    while (true) {
        udp::endpoint sender_endpoint;
        std::size_t bytes_received = socket.receive_from(boost::asio::buffer(buffer), sender_endpoint);

        std::string received_message(buffer, bytes_received);
        json received_data = json::parse(received_message);

        // ✅ Forward Data to WebSocket Clients
        for (auto hdl : connections) {
            server_instance.send(hdl, received_data.dump(), websocketpp::frame::opcode::text);
        }
    }
}


// ✅ Main Function
int main() {
    boost::asio::io_context io_context;

    std::thread ws_thread(run_websocket_server);
    std::thread udp_thread([&]() { udp_listener(io_context); });
    std::thread order_book_thread([&]() { broadcast_combined_data(io_context); });
    std::thread mock_order_thread([&]() { inject_mock_order_book(order_book); });
    std::thread trade_thread([&]() {
        while (true) {
            execute_trade(order_book);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    });

    ws_thread.join();
    udp_thread.join();
    order_book_thread.join();
    mock_order_thread.join();
    trade_thread.join();

    return 0;
}
