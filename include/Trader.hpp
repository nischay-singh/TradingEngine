#ifndef TRADER_HPP
#define TRADER_HPP

#include <string>
#include <vector>
#include <map>
#include <memory>

struct Trade
{
    std::string side;
    double price;
    int quantity;
    double timestamp;
};

class Trader
{
public:
    Trader(const std::string &name, double initialCapital);

    const std::string &getName() const { return name; }

    bool placeBuyLimitOrder(double price, int quantity);
    bool placeSellLimitOrder(double price, int quantity);
    bool placeBuyMarketOrder(int quantity);
    bool placeSellMarketOrder(int quantity);

    void updatePosition(const Trade &trade);
    double getCurrentPosition() const;
    double getUnrealizedPnL(double currentPrice) const;
    double getRealizedPnL() const;
    double getTotalPnL(double currentPrice) const;

    double getAvailableCapital() const;
    void updateCapital(double amount);

    const std::vector<Trade> &getTradeHistory() const;

private:
    std::string name;
    double capital;
    int position;
    double realizedPnL;
    std::vector<Trade> tradeHistory;

    bool validateOrder(double price, int quantity) const;
    std::string generateOrderId() const;
};

#endif // TRADER_HPP