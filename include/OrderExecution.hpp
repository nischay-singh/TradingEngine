#ifndef ORDER_EXECUTION_HPP
#define ORDER_EXECUTION_HPP

#include <string>

struct OrderExecution
{
  bool isExecuted;
  bool isPartiallyFilled;
  double avgPrice;
  int filledQuantity;
  int remainingQuantity;
  std::string status; // "FILLED", "PARTIALLY_FILLED", "NOT_FILLED"

  OrderExecution()
      : isExecuted(false),
        isPartiallyFilled(false),
        avgPrice(0.0),
        filledQuantity(0),
        remainingQuantity(0),
        status("NOT_FILLED") {}
};

#endif