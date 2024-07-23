#ifndef ORDER_H
#define ORDER_H

#include <string>

enum OrderType { BUY, SELL };

class Order {
  public:
      std::string id;
      int quantity;
      double price;
      OrderType type;

      Order(std::string id, double price, int quantity, OrderType type);
      void decrementQuantity(int matchedQuantity);
};

#endif

