#include "OrderManager.hpp"
#include <chrono>

OrderManager& OrderManager::getInstance() {
    static OrderManager instance;
    return instance;
}

OrderManager::OrderManager() {
    orderBook.initialize(100.0);
}

void OrderManager::registerTrader(std::shared_ptr<Trader> trader) {
    traders[trader->getName()] = trader;
}

void OrderManager::unregisterTrader(const std::string& traderId) {
    traders.erase(traderId);
}

bool OrderManager::submitOrder(const std::string& traderId, const Order& order) {
    auto it = traders.find(traderId);
    if (it == traders.end()) {
        return false;
    }
    
    OrderExecution execution = orderBook.processOrder(order);
    
    if (execution.isExecuted) {
        Trade trade;
        trade.side = (order.type == "buy_limit" || order.type == "buy_market") ? "buy" : "sell";
        trade.price = execution.avgPrice;
        trade.quantity = execution.filledQuantity;
        trade.timestamp = std::chrono::system_clock::now().time_since_epoch().count();
        
        it->second->updatePosition(trade);
    }
    
    return true;
}

void OrderManager::cancelOrder(const std::string& traderId, const std::string& orderId) {
    // Implementation for order cancellation
    // This would need to be added to OrderBook first
}

void OrderManager::updateMarketData(double price) {
    orderBook.updatePriceArtificially();
}

std::map<std::string, double> OrderManager::getMarketData() const {
    return orderBook.getMarketData();
} 