#include "../include/OrderManager.hpp"
#include <cassert>
#include <iostream>

void test_register_and_submit_order()
{
    OrderManager &mgr = OrderManager::getInstance();
    auto traderPtr = std::make_shared<Trader>("test_user", 100000);
    mgr.registerTrader(traderPtr);

    Order order;
    order.type = "buy_market";
    order.quantity = 10;
    order.price = 0.0;
    order.orderId = generateOrderId();

    auto result = mgr.submitOrder("test_user", order);
    assert(result && result->isExecuted);
    std::cout << "✅ test_register_and_submit_order passed\n";
}

void test_invalid_trader_submission()
{
    OrderManager &mgr = OrderManager::getInstance();

    Order order;
    order.type = "sell_market";
    order.quantity = 10;
    order.price = 0.0;
    order.orderId = generateOrderId();

    auto result = mgr.submitOrder("nonexistent", order);
    assert(!result.has_value());
    std::cout << "✅ test_invalid_trader_submission passed\n";
}

int main()
{
    test_register_and_submit_order();
    test_invalid_trader_submission();
    std::cout << "✅ All OrderManager tests passed.\n";
    return 0;
}