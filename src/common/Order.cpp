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

Order* Order::deserializeOrder(std::string str) {
    std::istringstream iss(str);
    std::string prefix, id, cust_id;
    double price;
    int quantity, type;

    std::getline(iss, prefix, ':');  // Read the "order:" prefix
    if (prefix != "order") {
        throw std::runtime_error("Invalid serialized order format");
    }

    std::getline(iss, id, ',');
    std::getline(iss, cust_id, ',');
    iss >> price;
    iss.ignore(1, ',');
    iss >> quantity;
    iss.ignore(1, ',');
    iss >> type;

    return new Order(id, cust_id, price, quantity, static_cast<OrderType>(type));
}
