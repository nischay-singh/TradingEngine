// #include "OrderManager.hpp"
// #include "Trader.hpp"
// #include <iostream>
// #include <thread>
// #include <chrono>

// void priceUpdateThread(OrderManager &orderManager)
// {
//     while (true)
//     {
//         orderManager.updatePrice();
//         std::this_thread::sleep_for(std::chrono::seconds(1));
//     }
// }

// int main()
// {
//     OrderManager &orderManager = OrderManager::getInstance();

//     Trader trader("TRADER1", 100000.0);
//     orderManager.registerTrader(std::make_shared<Trader>(trader));

//     std::thread priceThread(priceUpdateThread, std::ref(orderManager));

//     while (true)
//     {
//         auto marketData = orderManager.getMarketData();
//         double bestBid = marketData["best_bid"];
//         double bestAsk = marketData["best_ask"];
//         double midPrice = marketData["mid_price"];

//         std::cout << "\nCurrent Market Data:" << std::endl;
//         std::cout << "Best Bid: " << bestBid << std::endl;
//         std::cout << "Best Ask: " << bestAsk << std::endl;
//         std::cout << "Current Price: " << midPrice << std::endl;

//         bool orderFill = trader.placeBuyMarketOrder(100);
//         if (orderFill)
//         {
//             std::cout << "Order submitted successfully" << std::endl;
//         }
//         else
//         {
//             std::cout << "Failed to submit order" << std::endl;
//         }

//         std::cout << "Current Position: " << trader.getCurrentPosition() << std::endl;
//         std::cout << "Unrealized PnL: " << trader.getUnrealizedPnL(midPrice) << std::endl;
//         std::cout << "Realized PnL: " << trader.getRealizedPnL() << std::endl;

//         std::this_thread::sleep_for(std::chrono::seconds(5));
//     }

//     priceThread.join();

//     return 0;
// }
#include <QApplication>
#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(300, 150);
    w.show();
    return app.exec();
}