# Trading Engine

A real-time C++ trading engine with a Qt-based GUI, simulating a realistic market environment with order matching, market data streaming, PnL tracking, and live order book visualization.

## Features

- Real-time Order Book (bid/ask aggregation)
- Market and Limit order support
- Price simulation and continuous market updates
- Trader interface for order submission and performance tracking
- Real-time PnL tracking (realized and unrealized)
- Trade history table
- Order book visualization (top 5 bid/ask levels)
- Qt GUI with line chart display
- Thread-safe core (OrderManager and OrderBook)
- Modular and extensible C++ design

## Project Structure

```
TradingEngine/
├── include/             # Header files
│   ├── OrderBook.hpp
│   ├── OrderManager.hpp
│   ├── Trader.hpp
│   └── OrderExecution.hpp
├── src/                 # Source files
│   ├── OrderBook.cpp
│   ├── OrderManager.cpp
│   ├── Trader.cpp
│   ├── MainWindow.cpp
│   └── main.cpp
├── tests/               # Unit tests
├── CMakeLists.txt       # Build configuration
└── README.md            # This file
```

## Dependencies

- C++17 or later
- Qt 6 (Charts module required)
- CMake 3.10 or later

## Building the Project

Clone the repository and compile:

```bash
git clone https://github.com/nischay-singh/TradingEngine.git
cd TradingEngine
mkdir build && cd build
cmake ..
make
```

## Running the Project

To launch the main GUI application:

```bash
./TradingEngine
```

To run the tests:

```bash
./TradingEngineTests
```

## GUI Overview

- Chart: Live price chart (line chart).
- Labels: Display best bid, best ask, mid-price, current position, realized PnL, and unrealized PnL.
- Controls: Submit Buy/Sell Market and Limit Orders with price and quantity input.
- Order Book Table: Visualizes top 5 bid and ask levels.
- Trade History Table: Shows all executed trades.

## Thread Safety

The following components are protected with std::mutex to ensure multi-threaded access is safe:

- OrderManager
- OrderBook

## Usage Example

The trading engine simulates a real market environment where:

- Prices update every second
- Traders can submit market orders
- The order book matches orders based on price and time priority
- Market data is continuously updated and displayed

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request
