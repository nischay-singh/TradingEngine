#include "OrderBook.hpp"
#include <iostream>
#include <random>
#include <sstream>
#include <chrono>
#include <algorithm>

static std::mt19937 rng(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()));

OrderBook::OrderBook() : midPrice(100.0)
{
    initialize(midPrice);
}

void OrderBook::initialize(double midPrice)
{
    this->midPrice = midPrice;
    bidBook.clear();
    askBook.clear();

    // Create 5 levels of liquidity on each side.
    for (int i = 1; i <= 5; ++i)
    {
        double bidLevel = midPrice - i;
        double askLevel = midPrice + i;

        Order bidOrder;
        bidOrder.price = bidLevel;
        bidOrder.quantity = 100 * i;
        bidOrder.type = "buy_limit";
        bidBook[bidLevel].push_back(bidOrder);

        Order askOrder;
        askOrder.price = askLevel;
        askOrder.quantity = 100 * i;
        askOrder.type = "sell_limit";
        askBook[askLevel].push_back(askOrder);
    }
    updateBestPrices();
}

void OrderBook::refreshLiquidity(double midPrice)
{
    bidBook.clear();
    askBook.clear();

    std::uniform_real_distribution<double> priceOffsetDist(0.5, 1.5);
    std::uniform_int_distribution<int> volumeDist(50, 150);

    for (int i = 1; i <= 5; ++i)
    {
        double bidOffset = i * priceOffsetDist(rng);
        double askOffset = i * priceOffsetDist(rng);
        double bidLevel = midPrice - bidOffset;
        double askLevel = midPrice + askOffset;
        int bidVolume = volumeDist(rng);
        int askVolume = volumeDist(rng);

        Order bidOrder;
        bidOrder.price = bidLevel;
        bidOrder.quantity = bidVolume;
        bidOrder.type = "buy_limit";
        bidBook[bidLevel].push_back(bidOrder);

        Order askOrder;
        askOrder.price = askLevel;
        askOrder.quantity = askVolume;
        askOrder.type = "sell_limit";
        askBook[askLevel].push_back(askOrder);
    }
    this->midPrice = midPrice;
    updateBestPrices();
}

void OrderBook::updatePriceArtificially()
{
    std::uniform_real_distribution<double> movementDist(-1.0, 1.0);
    double movement = movementDist(rng);
    midPrice += movement;
    bidBook.clear();
    askBook.clear();
    for (int i = 1; i <= 5; ++i)
    {
        double bidLevel = midPrice - i;
        double askLevel = midPrice + i;

        Order bidOrder;
        bidOrder.price = bidLevel;
        bidOrder.quantity = 100 * i;
        bidOrder.type = "buy_limit";
        bidBook[bidLevel].push_back(bidOrder);

        Order askOrder;
        askOrder.price = askLevel;
        askOrder.quantity = 100 * i;
        askOrder.type = "sell_limit";
        askBook[askLevel].push_back(askOrder);
    }
    updateBestPrices();
}

OrderExecution OrderBook::processOrder(const Order &order)
{
    OrderExecution execution;

    if (order.type == "buy_limit")
    {
        if (askBook.empty() || order.price < askBook.begin()->first)
        {
            execution.status = "NOT_FILLED";
            return execution;
        }

        int remaining = order.quantity;
        double totalCost = 0;
        int totalFilled = 0;

        while (remaining > 0 && !askBook.empty() && order.price >= askBook.begin()->first)
        {
            double levelPrice = askBook.begin()->first;
            auto &ordersAtLevel = askBook.begin()->second;
            for (auto it = ordersAtLevel.begin(); it != ordersAtLevel.end() && remaining > 0;)
            {
                int available = it->quantity;
                int fill = std::min(remaining, available);
                remaining -= fill;
                totalFilled += fill;
                totalCost += fill * levelPrice;
                it->quantity -= fill;
                if (it->quantity == 0)
                {
                    it = ordersAtLevel.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            if (ordersAtLevel.empty())
            {
                askBook.erase(levelPrice);
            }
        }

        updateBestPrices();

        if (totalFilled > 0)
        {
            execution.isExecuted = true;
            execution.avgPrice = totalCost / totalFilled;
            execution.filledQuantity = totalFilled;
            execution.remainingQuantity = remaining;
            execution.isPartiallyFilled = (remaining > 0);
            execution.status = execution.isPartiallyFilled ? "PARTIALLY_FILLED" : "FILLED";
        }
        else
        {
            execution.status = "NOT_FILLED";
        }
    }
    else if (order.type == "sell_limit")
    {
        if (bidBook.empty() || order.price > bidBook.rbegin()->first)
        {
            execution.status = "NOT_FILLED";
            return execution;
        }

        int remaining = order.quantity;
        double totalProceeds = 0;
        int totalFilled = 0;

        while (remaining > 0 && !bidBook.empty() && order.price <= bidBook.rbegin()->first)
        {
            double levelPrice = bidBook.rbegin()->first;
            auto &ordersAtLevel = bidBook.rbegin()->second;
            for (auto it = ordersAtLevel.begin(); it != ordersAtLevel.end() && remaining > 0;)
            {
                int available = it->quantity;
                int fill = std::min(remaining, available);
                remaining -= fill;
                totalFilled += fill;
                totalProceeds += fill * levelPrice;
                it->quantity -= fill;
                if (it->quantity == 0)
                {
                    it = ordersAtLevel.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            if (bidBook.find(levelPrice) != bidBook.end() && bidBook[levelPrice].empty())
            {
                bidBook.erase(levelPrice);
            }
        }

        updateBestPrices();

        if (totalFilled > 0)
        {
            execution.isExecuted = true;
            execution.avgPrice = totalProceeds / totalFilled;
            execution.filledQuantity = totalFilled;
            execution.remainingQuantity = remaining;
            execution.isPartiallyFilled = (remaining > 0);
            execution.status = execution.isPartiallyFilled ? "PARTIALLY_FILLED" : "FILLED";
        }
        else
        {
            execution.status = "NOT_FILLED";
        }
    }
    else if (order.type == "buy_market")
    {
        if (askBook.empty())
        {
            execution.status = "NOT_FILLED";
            return execution;
        }

        int remaining = order.quantity;
        double totalCost = 0;
        int totalFilled = 0;

        while (remaining > 0 && !askBook.empty())
        {
            double levelPrice = askBook.begin()->first;
            auto &ordersAtLevel = askBook.begin()->second;
            for (auto it = ordersAtLevel.begin(); it != ordersAtLevel.end() && remaining > 0;)
            {
                int available = it->quantity;
                int fill = std::min(remaining, available);
                remaining -= fill;
                totalFilled += fill;
                totalCost += fill * levelPrice;
                it->quantity -= fill;
                if (it->quantity == 0)
                {
                    it = ordersAtLevel.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            if (ordersAtLevel.empty())
            {
                askBook.erase(levelPrice);
            }
        }

        updateBestPrices();

        if (totalFilled > 0)
        {
            execution.isExecuted = true;
            execution.avgPrice = totalCost / totalFilled;
            execution.filledQuantity = totalFilled;
            execution.remainingQuantity = remaining;
            execution.isPartiallyFilled = (remaining > 0);
            execution.status = execution.isPartiallyFilled ? "PARTIALLY_FILLED" : "FILLED";
        }
        else
        {
            execution.status = "NOT_FILLED";
        }
    }
    else if (order.type == "sell_market")
    {
        if (bidBook.empty())
        {
            execution.status = "NOT_FILLED";
            return execution;
        }

        int remaining = order.quantity;
        double totalProceeds = 0;
        int totalFilled = 0;

        while (remaining > 0 && !bidBook.empty())
        {
            double levelPrice = bidBook.rbegin()->first;
            auto &ordersAtLevel = bidBook.rbegin()->second;
            for (auto it = ordersAtLevel.begin(); it != ordersAtLevel.end() && remaining > 0;)
            {
                int available = it->quantity;
                int fill = std::min(remaining, available);
                remaining -= fill;
                totalFilled += fill;
                totalProceeds += fill * levelPrice;
                it->quantity -= fill;
                if (it->quantity == 0)
                {
                    it = ordersAtLevel.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            if (bidBook.find(levelPrice) != bidBook.end() && bidBook[levelPrice].empty())
            {
                bidBook.erase(levelPrice);
            }
        }

        updateBestPrices();

        if (totalFilled > 0)
        {
            execution.isExecuted = true;
            execution.avgPrice = totalProceeds / totalFilled;
            execution.filledQuantity = totalFilled;
            execution.remainingQuantity = remaining;
            execution.isPartiallyFilled = (remaining > 0);
            execution.status = execution.isPartiallyFilled ? "PARTIALLY_FILLED" : "FILLED";
        }
        else
        {
            execution.status = "NOT_FILLED";
        }
    }

    return execution;
}

double OrderBook::getBestBid() const
{
    return bestBid;
}

double OrderBook::getBestAsk() const
{
    return bestAsk;
}

std::map<std::string, double> OrderBook::getMarketData() const
{
    std::map<std::string, double> marketData;
    marketData["best_bid"] = bestBid;
    marketData["best_ask"] = bestAsk;
    marketData["mid_price"] = midPrice;
    return marketData;
}

void OrderBook::updateBestPrices()
{
    bestBid = bidBook.empty() ? 0.0 : bidBook.rbegin()->first;
    bestAsk = askBook.empty() ? 0.0 : askBook.begin()->first;
}