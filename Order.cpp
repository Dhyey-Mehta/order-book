#include "Order.h"

Order::Order(std::string id, double price, int quantity, OrderType type) : 
  id(id), price(price), quantity(quantity), type(type) {}

void Order::decrementQuantity(int matchedQuantity) {
    this->quantity -= matchedQuantity;
}

