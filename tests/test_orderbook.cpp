#include "../include/OrderBook.hpp"
#include <cassert>
#include <iostream>

void test_limit_buy_order()
{
    OrderBook book;
    Order buyOrder{"ORD1", "buy_limit", book.getBestAsk() + 1.0, 100};
    OrderExecution exec = book.processOrder(buyOrder);
    assert(exec.isExecuted);
    assert(exec.filledQuantity > 0);
    std::cout << "✅ test_limit_buy_order passed\n";
}

void test_limit_sell_order()
{
    OrderBook book;
    Order sellOrder{"ORD2", "sell_limit", book.getBestBid() - 1.0, 100};
    OrderExecution exec = book.processOrder(sellOrder);
    assert(exec.isExecuted);
    assert(exec.filledQuantity > 0);
    std::cout << "✅ test_limit_sell_order passed\n";
}

void test_market_buy_order()
{
    OrderBook book;
    Order buyOrder{"ORD3", "buy_market", 0.0, 100};
    OrderExecution exec = book.processOrder(buyOrder);
    assert(exec.isExecuted);
    assert(exec.filledQuantity > 0);
    std::cout << "✅ test_market_buy_order passed\n";
}

void test_market_sell_order()
{
    OrderBook book;
    Order sellOrder{"ORD4", "sell_market", 0.0, 100};
    OrderExecution exec = book.processOrder(sellOrder);
    assert(exec.isExecuted);
    assert(exec.filledQuantity > 0);
    std::cout << "✅ test_market_sell_order passed\n";
}

int main()
{
    test_limit_buy_order();
    test_limit_sell_order();
    test_market_buy_order();
    test_market_sell_order();
    std::cout << "✅ All OrderBook tests passed.\n";
    return 0;
}