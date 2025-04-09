# Trading Engine

A high-performance C++ trading engine implementation that simulates a real-time order book and trading system.

## Features

- Real-time order book management
- Market order and limit order support
- Order matching engine
- Trader interface for order submission
- Market data updates
- Thread-safe implementation
- Test suite for core functionality

## Project Structure

```
TradingEngine/
├── include/           # Header files
│   ├── OrderBook.hpp
│   ├── OrderManager.hpp
│   └── Trader.hpp
├── src/              # Source files
│   ├── OrderBook.cpp
│   ├── OrderManager.cpp
│   ├── Trader.cpp
│   └── main.cpp
├── tests/            # Test files
├── CMakeLists.txt    # Build configuration
└── README.md         # This file
```

## Dependencies

- C++11 or later
- CMake 3.10 or later
- A C++ compiler with C++11 support

## Building the Project

1. Clone the repository:
```bash
git clone https://github.com/nischay-singh/TradingEngine.git
cd TradingEngine
```

2. Create a build directory and compile:
```bash
mkdir build
cd build
cmake ..
make
```

## Running the Project

After building, you can run the main trading engine:
```bash
./TradingEngine
```

To run the tests:
```bash
./TradingEngineTests
```

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
