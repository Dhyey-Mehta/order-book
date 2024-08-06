#ifndef BOOK_H
#define BOOK_H

#include "Limit.h"
#include "Order.h"
#include "Match.h"

#include <map>
#include <iostream>
#include <vector>

class Book {
private:
    std::map<double, Limit*> buy_limits;
    std::map<double, Limit*> sell_limits;
    std::map<std::string, Order*> order_map;
    std::vector<Match> match_order(Order* order);

public:
    Book();
    ~Book();
    Limit* find_or_create_limit(double price, OrderType type);
    std::vector<Match> new_order(Order* order);
    void delete_order(std::string order_id);
    void print_book();
};

#endif

