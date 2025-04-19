#include "OrderManager.hpp"
#include <chrono>
#include <iostream>
#include <sstream>
#include <chrono>
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
    traders[trader->getName()] = trader;
}

void OrderManager::unregisterTrader(const std::string &traderId)
{
    traders.erase(traderId);
}

std::optional<OrderExecution> OrderManager::submitOrder(const std::string &traderId, const Order &order)
{
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

void OrderManager::cancelOrder(const std::string &traderId, const std::string &orderId)
{
    // TODO

    // if (orderToTraderMap[orderId] == traderId)
    // {
    //     orderBook.cancelOrder(orderId);
    //     orderToTraderMap.erase(orderId);
    // }
}

void OrderManager::updatePrice()
{
    orderBook.updatePriceArtificially();
}

std::map<std::string, double> OrderManager::getMarketData() const
{
    return orderBook.getMarketData();
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