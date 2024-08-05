#include "Order.h"

Order::Order(std::string id, std::string cust_id, double price, int quantity, OrderType type) : 
  id(id), cust_id(cust_id), price(price), quantity(quantity), type(type) {}

void Order::decrementQuantity(int matchedQuantity) {
    quantity -= matchedQuantity;
}

std::string Order::serializeOrder() {
    std::ostringstream oss;
    oss << "order:"
        << id << ','
        << cust_id << ','
        << price << ','
        << quantity << ','
        << static_cast<int>(type);
    return oss.str();
}
