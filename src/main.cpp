#include "OrderManager.hpp"
#include "Trader.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <random>

// Function to update prices in the order book
void priceUpdateThread(OrderManager& orderManager) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> priceChange(-1.0, 1.0);
    
    while (true) {
        // Get current market data
        auto marketData = orderManager.getMarketData();
        double currentPrice = marketData["mid_price"];
        
        // Generate a small random price change
        double change = priceChange(gen);
        double newPrice = currentPrice + change;
        
        // Update the market data
        orderManager.updateMarketData(newPrice);
        
        // Sleep for 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    // Get the OrderManager instance
    OrderManager& orderManager = OrderManager::getInstance();
    
    // Create a trader
    auto trader = std::make_shared<Trader>("TRADER1");
    orderManager.registerTrader(trader);
    
    // Start the price update thread
    std::thread priceThread(priceUpdateThread, std::ref(orderManager));
    
    // Main trading loop
    while (true) {
        // Get current market data
        auto marketData = orderManager.getMarketData();
        double bestBid = marketData["best_bid"];
        double bestAsk = marketData["best_ask"];
        
        std::cout << "\nCurrent Market Data:" << std::endl;
        std::cout << "Best Bid: " << bestBid << std::endl;
        std::cout << "Best Ask: " << bestAsk << std::endl;
        
        // Create a market order
        Order order;
        order.orderId = generateOrderId();
        order.type = "buy_market";  // or "sell_market"
        order.price = 0.0;  // Market orders don't need a price
        order.quantity = 100;  // Example quantity
        
        // Submit the order
        bool success = orderManager.submitOrder("TRADER1", order);
        if (success) {
            std::cout << "Order submitted successfully: " << order.orderId << std::endl;
        } else {
            std::cout << "Failed to submit order" << std::endl;
        }
        
        // Sleep for 5 seconds before next order
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    
    // Join the price update thread (this will never be reached in this example)
    priceThread.join();
    
    return 0;
}