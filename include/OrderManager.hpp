#ifndef ORDER_MANAGER_HPP
#define ORDER_MANAGER_HPP

#include "OrderBook.hpp"
#include "Trader.hpp"
#include <string>
#include <vector>
#include <map>
#include <memory>

class OrderManager {
public:
    static OrderManager& getInstance();
    
    void registerTrader(std::shared_ptr<Trader> trader);
    void unregisterTrader(const std::string& traderId);
    
    bool submitOrder(const std::string& traderId, const Order& order);
    void cancelOrder(const std::string& traderId, const std::string& orderId);
    
    void updateMarketData(double price);
    std::map<std::string, double> getMarketData() const;
    
private:
    OrderManager();
    ~OrderManager() = default;
    OrderManager(const OrderManager&) = delete;
    OrderManager& operator=(const OrderManager&) = delete;
    
    OrderBook orderBook;
    std::map<std::string, std::shared_ptr<Trader>> traders;
    std::map<std::string, std::string> orderToTraderMap;
};

#endif 