#ifndef MATCH_H
#define MATCH_H

#include <string>
#include <sstream>


class Match {
public:
    std::string order_id1;
    std::string order_id2;
    double price;
    int quantity;
    double ask_price;
    double bid_price;

    Match(std::string order_id1, std::string order_id2, double price, int quantity, double ask_price, double bid_price);
    std::string serializeMatch();
};

#endif

