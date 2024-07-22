#include "Book.h"
#include "Order.h"
#include "Limit.h"
#include <iostream>

int main() {
    Book* book = new Book();
    book->new_order(new Order("order1", 100.0, 9, OrderType::BUY));
    book->new_order(new Order("order2", 100.0, 10, OrderType::SELL));
    
    book->print_book();
    
    delete book; 
    return 0;
}

