// Copyright 2020 lamp
// Created by lamp on 5/10/21.
//

#include <page_queue.hh>

page_queue* page_queue::get_instance(){
  static page_queue instance{};
  return &instance;
}

bool page_queue::pop(page& u) {
  std::lock_guard lock(_mutex);

  if (_queue.empty()) {
    return false;
  }

  u = std::move(_queue.front());
  _queue.pop();
  return true;
}

void page_queue::push(page&& u) {
  std::lock_guard lock(_mutex);
  _queue.push(std::move(u));
}

void page_queue::push(const page& u) {
  std::lock_guard lock(_mutex);
  _queue.push(u);
}
