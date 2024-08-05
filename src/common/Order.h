#ifndef ORDER_H
#define ORDER_H

#include <iostream>
#include <string>
#include <sstream>

enum OrderType { BUY, SELL };

class Order {
  public:
      std::string id;
      std::string cust_id;
      int quantity;
      double price;
      OrderType type;

      Order(std::string id, std::string cust_id, double price, int quantity, OrderType type);
      void decrementQuantity(int matchedQuantity);
      std::string serializeOrder();
};

#endif

