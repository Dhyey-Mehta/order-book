#include "Book.h"
#include "Order.h"
#include "Limit.h"

#include <iostream>
#include <stdlib.h>

int main() {
    Book* book = new Book();
    Order *order = new Order(
        "order",
        "cust1",
        10,
        25,
        OrderType::SELL
    );
    book->new_order(order);
    book->print_book();
    book->delete_order("order");
    book->print_book();


    // book->new_order( new Order(
    //     "order2",
    //     "cust2",
    //     10,
    //     25,
    //     OrderType::SELL)
    // );
        
    delete book; 
    return 0;
}

