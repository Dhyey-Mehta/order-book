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

    Match(std::string order_id1, std::string order_id2, double price, int quantity);
    std::string serializeMatch();
};

#endif

