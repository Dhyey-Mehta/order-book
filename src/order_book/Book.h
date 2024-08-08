#ifndef BOOK_H
#define BOOK_H

#include "Limit.h"
#include "Order.h"
#include "Match.h"

#include <queue>
#include <iostream>
#include <vector>
#include <unordered_map>

class Book {
private:
    std::priority_queue<std::pair<double, Limit*>> buy_heap;
    std::unordered_map<double, Limit*> buy_map;

    // make prices negative for sell heap to keep it a min heap
    std::priority_queue<std::pair<double, Limit*>> sell_heap;
    std::unordered_map<double, Limit*> sell_map;
    
    std::unordered_map<std::string, Order*> order_map;

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

