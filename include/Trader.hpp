#ifndef TRADER_HPP
#define TRADER_HPP

#include "OrderBook.hpp"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <deque>

struct Trade
{
    std::string side;
    double price;
    int quantity; // positive for long, negative for short
};

struct OpenPosition
{
    int quantity;
    double price;
};

class Trader
{
public:
    Trader(const std::string &name, double initialCapital);

    const std::string &getName() const { return name; }

    bool placeBuyLimitOrder(double price, int quantity);
    bool placeSellLimitOrder(double price, int quantity);
    bool placeBuyMarketOrder(double quantity);
    bool placeSellMarketOrder(double quantity);

    void updatePosition(const Trade &trade);
    double getCurrentPosition() const { return currentPosition; }
    double getUnrealizedPnL(double currentPrice) const;
    double getRealizedPnL() const { return realizedPnL; }
    double getTotalPnL(double currentPrice) const;

    double getAvailableCapital() const;
    void updateCapital(double amount);

    const std::vector<Trade> &getTradeHistory() const;

private:
    std::string name;
    double capital;
    double currentPosition;
    double realizedPnL;
    std::vector<Trade> tradeHistory;
    std::deque<OpenPosition> openPositions;

    bool validateOrder(double price, int quantity) const;
};

#endif // TRADER_HPP