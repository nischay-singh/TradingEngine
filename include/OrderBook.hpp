#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP

#include <map>
#include <string>
#include "OrderExecution.hpp"

struct Order
{
    std::string orderId;
    std::string type; // "buy_limit", "sell_limit", "buy_market", "sell_market"
    double price;
    int quantity;
};

class OrderBook
{
public:
    OrderBook();

    void updatePriceArtificially();

    OrderExecution processOrder(const Order &order);

    double getBestBid() const;
    double getBestAsk() const;

    std::map<std::string, double> getMarketData() const;

    void initialize(double midPrice);
    void refreshLiquidity(double midPrice);

private:
    double midPrice;

    std::map<double, std::vector<Order>> bidBook;
    std::map<double, std::vector<Order>> askBook;
    double bestBid;
    double bestAsk;

    void updateBestPrices();
};

#endif // ORDER_BOOK_HPP