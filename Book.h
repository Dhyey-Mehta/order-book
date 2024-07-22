#ifndef BOOK_H
#define BOOK_H

#include "Limit.h"
#include "Order.h"
#include <map>
#include <iostream>

class Book {
private:
    std::map<double, Limit*> buy_limits;
    std::map<double, Limit*> sell_limits;
    void match_order(Order* order);

public:
    Book();
    ~Book();
    Limit* find_or_create_limit(double price, OrderType type);
    void new_order(Order* order);
    void print_book();
};

#endif

