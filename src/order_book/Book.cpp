#include "Book.h"

Book::Book() {}

Book::~Book() {
    for (auto& pair : buy_limits) {
        delete pair.second;
    }
    for (auto& pair : sell_limits) {
        delete pair.second;
    }
    buy_limits.clear();
    sell_limits.clear();
}

Limit* Book::find_or_create_limit(double price, OrderType type) {
  std::map<double, Limit*>* limits = nullptr;
  switch(type) {
    case OrderType::BUY:
      limits = &buy_limits;
      break;
    case OrderType::SELL:
      limits = &sell_limits;
      break;
  }

  auto it = limits->find(price);
  if (it != limits->end()) {
    return it->second;
  }
  
  Limit *new_limit = new Limit(price);
  (*limits)[price] = new_limit;
  return new_limit;
}

std::vector<Match> Book::new_order(Order* order) {
  std::cout << order->serializeOrder() << std::endl;
  std::vector<Match> fulfillments = match_order(order);

  // add order if there is any quantity left
  if (order -> quantity > 0) {
    Limit *limit = find_or_create_limit(order->price, order->type);
    limit->add_order(order);
    order_map[order->id] = order;
  }
  return fulfillments;
}

void Book::delete_order(std::string order_id) {
  // TODO: do checking here to check if id exists in order_map
  Order *to_remove = order_map[order_id];
  Limit *limit = find_or_create_limit(to_remove->price, to_remove->type);
  limit->remove_order(order_id);
}

std::vector<Match> Book::match_order(Order* order) {
  std::map<double, Limit*>* limits = nullptr;
  switch(order->type) {
    case OrderType::BUY:
      limits = &sell_limits;
      break;
    case OrderType::SELL:
      limits = &buy_limits;
      break;
  }
  std::vector<Match> fulfillments;
  auto limit_pair = limits->find(order->price);
  if (limit_pair == limits->end()) { return fulfillments; }

  Limit *curr_limit = limit_pair->second;

  while (curr_limit->total_volume > 0 && order->quantity > 0) {
    Order *to_be_matched = curr_limit->orders.front();
    int quantity_matched = std::min(
        to_be_matched->quantity,
        order->quantity
    );

    // adjust quantity as orders are matched
    to_be_matched->quantity -= quantity_matched;
    order->quantity -= quantity_matched;
    curr_limit->total_volume -= quantity_matched;
    Match matched(order->id, to_be_matched->id, order->price, quantity_matched);
    std::cout << matched.serializeMatch() << std::endl;
    fulfillments.push_back(matched);

    if (to_be_matched->quantity == 0) {
      curr_limit->orders.pop_front();
      delete to_be_matched;
    }
  }

  return fulfillments;
}

void Book::print_book() {
  std::cout << "____________________________________________________________" << std::endl;
  std::cout << "BUY LIMITS:" << std::endl;
  for (auto& pair : buy_limits) {
    std::cout << "  price: " << pair.second->price;
    std::cout << "  quantity: " << pair.second->total_volume << std::endl;
  }

  std::cout << "SELL LIMITS:" << std::endl;
  for (auto& pair : sell_limits) {
    std::cout << "  price: " << pair.second->price;
    std::cout << "  quantity: " << pair.second->total_volume << std::endl;
  }
  std::cout << "____________________________________________________________" << std::endl;
}

