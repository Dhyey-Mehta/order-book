#include "Book.h"

Book::Book() {}

Book::~Book() {
    for (auto& pair : buy_map) {
        delete pair.second;
    }
    for (auto& pair : sell_map) {
        delete pair.second;
    }
    buy_map.clear();
    sell_map.clear();
}

Limit* Book::find_or_create_limit(double price, OrderType type) {
  std::unordered_map<double, Limit*>* limits_map = nullptr;
  std::priority_queue<std::pair<double, Limit*>>* limits_heap = nullptr;
  double price_in_heap = price;

  switch(type) {
    case OrderType::BUY:
      limits_map = &buy_map;
      limits_heap = &buy_heap;
      break;
    case OrderType::SELL:
      limits_map = &sell_map;
      limits_heap = &sell_heap;
      price_in_heap = -price;
      break;
  }

  auto it = limits_map->find(price);
  if (it != limits_map->end()) {
    return it->second;
  }
  
  Limit *new_limit = new Limit(price);
  (*limits_map)[price] = new_limit;
  limits_heap->push(std::pair(price_in_heap, new_limit));

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
  std::priority_queue<std::pair<double, Limit*>>* limits = nullptr;
  std::unordered_map<double, Limit*>* limits_map = nullptr;

  switch(order->type) {
    case OrderType::BUY:
      limits = &sell_heap;
      limits_map = &sell_map;
      break;
    case OrderType::SELL:
      limits = &buy_heap;
      limits_map = &buy_map;
      break;
  }
  std::vector<Match> fulfillments;

  while (!limits->empty() && order->quantity > 0) {
    Limit *curr_limit = limits->top().second;
    bool is_tradeable = order->type == OrderType::BUY ? curr_limit->price <= order->price : order->price <= curr_limit->price;
    if (!is_tradeable) { break; }

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
      double ask_price = order->type == OrderType::BUY ? to_be_matched->price : order->price;
      double bid_price = order->type == OrderType::BUY ? order->price : to_be_matched->price;
      Match matched(order->id, to_be_matched->id, order->price, quantity_matched, ask_price, bid_price);
      std::cout << matched.serializeMatch() << std::endl;
      fulfillments.push_back(matched);

      if (to_be_matched->quantity == 0) {
        curr_limit->orders.pop_front();
        // TODO: remove from orders map
        delete to_be_matched;
      }
    }

    if (curr_limit->total_volume == 0) {
      limits->pop();
      limits_map->erase(curr_limit->price);
      delete curr_limit;
    }
  }

  return fulfillments;
}

void Book::print_book() {
  std::cout << "____________________________________________________________" << std::endl;
  std::cout << "BUY LIMITS:" << std::endl;
  for (auto& pair : buy_map) {
    std::cout << "  price: " << pair.second->price;
    std::cout << "  quantity: " << pair.second->total_volume << std::endl;
  }

  std::cout << "SELL LIMITS:" << std::endl;
  for (auto& pair : sell_map) {
    std::cout << "  price: " << pair.second->price;
    std::cout << "  quantity: " << pair.second->total_volume << std::endl;
  }
  std::cout << "____________________________________________________________" << std::endl;
}

