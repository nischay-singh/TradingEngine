#include "OrderManager.hpp"
#include <chrono>
#include <iostream>
#include <sstream>
#include <random>

static std::mt19937 rng(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()));

OrderManager &OrderManager::getInstance()
{
    static OrderManager instance;
    return instance;
}

OrderManager::OrderManager()
{
    orderBook.initialize(100.0);
}

void OrderManager::registerTrader(std::shared_ptr<Trader> trader)
{
    std::lock_guard<std::mutex> lock(mutex);
    traders[trader->getName()] = trader;
}

void OrderManager::unregisterTrader(const std::string &traderId)
{
    std::lock_guard<std::mutex> lock(mutex);
    traders.erase(traderId);
}

std::optional<OrderExecution> OrderManager::submitOrder(const std::string &traderId, const Order &order)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (traders.find(traderId) == traders.end())
    {
        return std::nullopt;
    }

    auto execution = orderBook.processOrder(order);

    if (execution.isExecuted)
    {
        return execution;
    }

    return std::nullopt;
}

void OrderManager::updatePrice()
{
    std::lock_guard<std::mutex> lock(mutex);
    orderBook.updatePriceArtificially();
}

std::map<std::string, double> OrderManager::getMarketData() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return orderBook.getMarketData();
}

OrderBook &OrderManager::getOrderBook()
{
    return orderBook;
}

std::vector<Trade> OrderManager::getTraderTradeHistory(const std::string &traderId) const
{
    std::lock_guard<std::mutex> lock(mutex);
    if (traders.find(traderId) != traders.end())
    {
        return traders.at(traderId)->getTradeHistory();
    }
    return {};
}

std::string generateOrderId()
{
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    std::uniform_int_distribution<int> dist(1000, 9999);
    int random = dist(rng);

    std::stringstream ss;
    ss << "ORD" << timestamp << random;
    return ss.str();
}