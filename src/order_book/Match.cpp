#include "Match.h"

Match::Match(std::string order_id1, std::string order_id2, double price, int quantity): 
    order_id1(order_id1), order_id2(order_id2), price(price), quantity(quantity) {}

std::string Match::serializeMatch() {
    std::ostringstream oss;
    oss << "match:"
        << order_id1 << ','
        << order_id2 << ','
        << price << ','
        << quantity;
    return oss.str();
}