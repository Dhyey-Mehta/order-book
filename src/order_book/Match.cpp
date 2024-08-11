#include "Match.h"

Match::Match(std::string order_id1, std::string order_id2, double price, int quantity, double ask_price, double bid_price): 
    order_id1(order_id1), order_id2(order_id2), price(price), quantity(quantity), ask_price(ask_price), bid_price(bid_price) {}

std::string Match::serializeMatch() {
    std::ostringstream oss;
    oss << "match:"
        << order_id1 << ','
        << order_id2 << ','
        << price << ','
        << quantity << ','
        << ask_price << ','
        << bid_price;
    return oss.str();
}