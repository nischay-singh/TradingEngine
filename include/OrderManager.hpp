#ifndef ORDER_MANAGER_HPP
#define ORDER_MANAGER_HPP

#include "OrderBook.hpp"
#include "Trader.hpp"
#include "OrderExecution.hpp"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <chrono>
#include <optional>

std::string generateOrderId();

class OrderManager
{
public:
    static OrderManager &getInstance();

    void registerTrader(std::shared_ptr<Trader> trader);
    void unregisterTrader(const std::string &traderId);

    std::optional<OrderExecution> submitOrder(const std::string &traderId, const Order &order);
    void cancelOrder(const std::string &traderId, const std::string &orderId);

    void updatePrice();
    std::map<std::string, double> getMarketData() const;

    OrderBook &getOrderBook() { return orderBook; }

    std::vector<Trade> getTraderTradeHistory(const std::string &traderId) const;

private:
    OrderManager();
    ~OrderManager() = default;
    OrderManager(const OrderManager &) = delete;
    OrderManager &operator=(const OrderManager &) = delete;

    OrderBook orderBook;
    std::map<std::string, std::shared_ptr<Trader>> traders;
};

#endif