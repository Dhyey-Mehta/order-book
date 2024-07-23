#include "Book.h"
#include "Order.h"
#include "Limit.h"

#include <iostream>
#include <stdlib.h>

int main() {
    Book* book = new Book();
    book->new_order( new Order(
        "order",
        10,
        25,
        OrderType::BUY)
    );
    book->new_order( new Order(
        "order2",
        10,
        25,
        OrderType::SELL)
    );
    
    book->print_book();
    
    delete book; 
    return 0;
}

