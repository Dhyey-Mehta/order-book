#ifndef LIMIT_H
#define LIMIT_H

#include "Order.h"
#include <list>

class Limit {
public:
    double price;
    int total_volume;
    std::list<Order*> orders;

    ~Limit();
    Limit(double price);
    void add_order(Order* order);
};

#endif

