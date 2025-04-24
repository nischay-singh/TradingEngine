#include "../include/Trader.hpp"
#include "../include/OrderManager.hpp"
#include <cassert>
#include <iostream>

void test_buy_limit_execution()
{
    auto trader = std::make_shared<Trader>("BuyLimit", 100000);
    OrderManager::getInstance().registerTrader(trader);

    auto mid = OrderManager::getInstance().getMarketData()["mid_price"];
    auto price = mid + 1.5;

    auto exec = trader->placeBuyLimitOrder(price, 10);
    assert(exec && exec->isExecuted);
    std::cout << "test_buy_limit_execution passed\n";
}

void test_sell_limit_execution()
{
    auto trader = std::make_shared<Trader>("SellLimit", 100000);
    OrderManager::getInstance().registerTrader(trader);

    auto mid = OrderManager::getInstance().getMarketData()["mid_price"];
    auto price = mid - 1.5;

    auto exec = trader->placeSellLimitOrder(price, 10);
    assert(exec && exec->isExecuted);
    std::cout << "test_sell_limit_execution passed\n";
}

void test_buy_market_execution()
{
    auto trader = std::make_shared<Trader>("BuyMarket", 100000);
    OrderManager::getInstance().registerTrader(trader);

    auto exec = trader->placeBuyMarketOrder(10);
    assert(exec && exec->isExecuted);
    std::cout << "test_buy_market_execution passed\n";
}

void test_sell_market_execution()
{
    auto trader = std::make_shared<Trader>("SellMarket", 100000);
    OrderManager::getInstance().registerTrader(trader);

    // First buy to hold some position
    auto exec1 = trader->placeBuyMarketOrder(10);
    assert(exec1 && exec1->isExecuted);

    auto exec2 = trader->placeSellMarketOrder(10);
    assert(exec2 && exec2->isExecuted);
    std::cout << "test_sell_market_execution passed\n";
}

int main()
{
    test_buy_limit_execution();
    test_sell_limit_execution();
    test_buy_market_execution();
    test_sell_market_execution();
    return 0;
}