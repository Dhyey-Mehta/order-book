#include "Limit.h"

Limit::Limit(double price) : price(price), total_volume(0) {}

Limit::~Limit() {
    for (Order* order : orders) {
        delete order;
    }
    orders.clear();
}

void Limit::add_order(Order* order) {
    orders.push_back(order);
    total_volume += order->quantity;
}

