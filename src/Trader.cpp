#include "Trader.hpp"
#include "OrderManager.hpp"
#include <chrono>
#include <algorithm>
#include <sstream>
#include <random>
#include <iostream>

Trader::Trader(const std::string &name, double initialCapital)
    : name(name), capital(initialCapital), currentPosition(0.0), realizedPnL(0.0) {}

bool Trader::placeBuyLimitOrder(double price, int quantity)
{
    if (!validateOrder(price, quantity))
    {
        return false;
    }

    Order order;
    order.type = "buy_limit";
    order.price = price;
    order.quantity = quantity;
    order.orderId = generateOrderId();

    auto execution = OrderManager::getInstance().submitOrder(name, order);
    if (execution && execution->isExecuted)
    {
        Trade trade;
        trade.side = "buy";
        trade.price = execution->avgPrice;
        trade.quantity = execution->filledQuantity;
        tradeHistory.push_back(trade);

        updatePosition(trade);
        return true;
    }
    return false;
}

bool Trader::placeSellLimitOrder(double price, int quantity)
{
    if (!validateOrder(price, quantity))
    {
        return false;
    }

    Order order;
    order.type = "sell_limit";
    order.price = price;
    order.quantity = quantity;
    order.orderId = generateOrderId();

    auto execution = OrderManager::getInstance().submitOrder(name, order);
    if (execution && execution->isExecuted)
    {
        Trade trade;
        trade.side = "sell";
        trade.price = execution->avgPrice;
        trade.quantity = execution->filledQuantity;
        tradeHistory.push_back(trade);

        updatePosition(trade);
        return true;
    }
    return false;
}

bool Trader::placeBuyMarketOrder(double quantity)
{
    Order order;
    order.orderId = generateOrderId();
    order.type = "buy_market";
    order.quantity = quantity;
    order.price = 0.0;

    auto execution = OrderManager::getInstance().submitOrder(name, order);
    if (execution && execution->isExecuted)
    {
        Trade trade;
        trade.side = "buy";
        trade.price = execution->avgPrice;
        trade.quantity = execution->filledQuantity;
        tradeHistory.push_back(trade);

        updatePosition(trade);
        return true;
    }
    return false;
}

bool Trader::placeSellMarketOrder(double quantity)
{
    Order order;
    order.orderId = generateOrderId();
    order.type = "sell_market";
    order.quantity = quantity;
    order.price = 0.0;

    auto execution = OrderManager::getInstance().submitOrder(name, order);
    if (execution && execution->isExecuted)
    {
        Trade trade;
        trade.side = "sell";
        trade.price = execution->avgPrice;
        trade.quantity = execution->filledQuantity;
        tradeHistory.push_back(trade);

        updatePosition(trade);
        return true;
    }
    return false;
}

void Trader::updatePosition(const Trade &trade)
{
    if (trade.side == "buy")
    {
        if (currentPosition < 0)
        {
            int coverQuantity = trade.quantity;
            while (coverQuantity > 0 && !openPositions.empty() && openPositions.front().quantity < 0)
            {
                int shortQty = -openPositions.front().quantity;
                int matched = std::min(coverQuantity, shortQty);

                realizedPnL += matched * (openPositions.front().price - trade.price);
                openPositions.front().quantity += matched;
                coverQuantity -= matched;
                if (openPositions.front().quantity == 0)
                {
                    openPositions.pop_front();
                }
            }
            if (coverQuantity > 0)
            {
                openPositions.push_back(OpenPosition{coverQuantity, trade.price});
            }
            currentPosition += trade.quantity;
            capital -= trade.price * trade.quantity;
        }
        else
        {
            openPositions.push_back(OpenPosition{trade.quantity, trade.price});
            currentPosition += trade.quantity;
            capital -= trade.price * trade.quantity;
        }
    }
    else if (trade.side == "sell")
    {
        if (currentPosition > 0)
        {
            int sellQuantity = trade.quantity;
            while (sellQuantity > 0 && !openPositions.empty() && openPositions.front().quantity > 0)
            {
                int matched = std::min(sellQuantity, openPositions.front().quantity);
                realizedPnL += matched * (trade.price - openPositions.front().price);
                openPositions.front().quantity -= matched;
                sellQuantity -= matched;
                if (openPositions.front().quantity == 0)
                {
                    openPositions.pop_front();
                }
            }
            if (sellQuantity > 0)
            {
                openPositions.push_back(OpenPosition{-sellQuantity, trade.price});
            }
            currentPosition -= trade.quantity;
            capital += trade.price * trade.quantity;
        }
        else
        {
            openPositions.push_back(OpenPosition{-trade.quantity, trade.price});
            currentPosition -= trade.quantity;
            capital += trade.price * trade.quantity;
        }
    }
    tradeHistory.push_back(trade);
}

double Trader::getUnrealizedPnL(double currentPrice) const
{
    double pnl = 0.0;
    for (const auto &pos : openPositions)
    {
        if (pos.quantity > 0)
        {
            pnl += pos.quantity * (currentPrice - pos.price);
        }
        else if (pos.quantity < 0)
        {
            pnl += (-pos.quantity) * (pos.price - currentPrice);
        }
    }
    return pnl;
}

double Trader::getTotalPnL(double currentPrice) const
{
    return getRealizedPnL() + getUnrealizedPnL(currentPrice);
}

double Trader::getAvailableCapital() const
{
    return capital;
}

void Trader::updateCapital(double amount)
{
    capital += amount;
}

const std::vector<Trade> &Trader::getTradeHistory() const
{
    return tradeHistory;
}

bool Trader::validateOrder(double price, int quantity) const
{
    if (price <= 0 || quantity <= 0)
    {
        return false;
    }

    if (price * quantity > capital)
    {
        return false;
    }

    return true;
}