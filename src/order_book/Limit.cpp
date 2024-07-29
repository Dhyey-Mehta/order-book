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

void Limit::remove_order(const std::string order_id) {
    for (auto it = orders.begin(); it != orders.end(); ++it) {
        if ((*it)->id == order_id) {
            total_volume -= (*it)->quantity;
            delete *it;
            orders.erase(it);
            break;
        }
    }
}
