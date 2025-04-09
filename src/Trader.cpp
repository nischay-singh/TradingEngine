#include "Trader.hpp"
#include "OrderManager.hpp"
#include <chrono>
#include <algorithm>
#include <sstream>
#include <random>

Trader::Trader(const std::string& name, double initialCapital)
    : name(name), capital(initialCapital), position(0), realizedPnL(0.0) {}

bool Trader::placeBuyLimitOrder(double price, int quantity) {
    if (!validateOrder(price, quantity)) {
        return false;
    }
    
    Order order;
    order.type = "buy_limit";
    order.price = price;
    order.quantity = quantity;
    order.orderId = generateOrderId();
    
    return OrderManager::getInstance().submitOrder(name, order);
}

bool Trader::placeSellLimitOrder(double price, int quantity) {
    if (!validateOrder(price, quantity)) {
        return false;
    }
    
    Order order;
    order.type = "sell_limit";
    order.price = price;
    order.quantity = quantity;
    order.orderId = generateOrderId();
    
    return OrderManager::getInstance().submitOrder(name, order);
}

bool Trader::placeBuyMarketOrder(int quantity) {
    if (quantity <= 0) {
        return false;
    }
    
    Order order;
    order.type = "buy_market";
    order.quantity = quantity;
    order.price = 0.0;
    order.orderId = generateOrderId();
    
    return OrderManager::getInstance().submitOrder(name, order);
}

bool Trader::placeSellMarketOrder(int quantity) {
    if (quantity <= 0) {
        return false;
    }
    
    Order order;
    order.type = "sell_market";
    order.quantity = quantity;
    order.price = 0.0;
    order.orderId = generateOrderId();
    
    return OrderManager::getInstance().submitOrder(name, order);
}

void Trader::updatePosition(const Trade& trade) {
    if (trade.side == "buy") {
        position += trade.quantity;
        capital -= trade.price * trade.quantity;
    } else {
        position -= trade.quantity;
        capital += trade.price * trade.quantity;
    }
    
    if (trade.side == "sell") {
        int remainingQuantity = trade.quantity;
        auto it = tradeHistory.begin();
        while (remainingQuantity > 0 && it != tradeHistory.end()) {
            if (it->side == "buy") {
                int matchedQuantity = std::min(remainingQuantity, it->quantity);
                realizedPnL += matchedQuantity * (trade.price - it->price);
                remainingQuantity -= matchedQuantity;
            }
            ++it;
        }
    }
    
    tradeHistory.push_back(trade);
}

double Trader::getCurrentPosition() const {
    return position;
}

double Trader::getUnrealizedPnL(double currentPrice) const {
    if (position == 0) return 0.0;
    
    double avgEntryPrice = 0.0;
    int totalQuantity = 0;
    
    for (const auto& trade : tradeHistory) {
        if (trade.side == "buy") {
            avgEntryPrice += trade.price * trade.quantity;
            totalQuantity += trade.quantity;
        }
    }
    
    if (totalQuantity == 0) return 0.0;
    avgEntryPrice /= totalQuantity;
    
    return position * (currentPrice - avgEntryPrice);
}

double Trader::getRealizedPnL() const {
    return realizedPnL;
}

double Trader::getTotalPnL(double currentPrice) const {
    return getRealizedPnL() + getUnrealizedPnL(currentPrice);
}

double Trader::getAvailableCapital() const {
    return capital;
}

void Trader::updateCapital(double amount) {
    capital += amount;
}

const std::vector<Trade>& Trader::getTradeHistory() const {
    return tradeHistory;
}

bool Trader::validateOrder(double price, int quantity) const {
    if (price <= 0 || quantity <= 0) {
        return false;
    }
    
    if (price * quantity > capital) {
        return false;
    }
    
    return true;
}

std::string Trader::generateOrderId() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static const char* hex_chars = "0123456789ABCDEF";
    
    std::string orderId;
    for (int i = 0; i < 8; ++i) {
        orderId += hex_chars[dis(gen)];
    }
    return name + "_" + orderId;
} 