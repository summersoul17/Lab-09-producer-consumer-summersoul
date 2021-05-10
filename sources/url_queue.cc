// Copyright 2020 lamp
// Created by lamp on 5/10/21.
//

#include <url_queue.hh>

url_queue* url_queue::get_instance(){
  static url_queue instance{};
  return &instance;
}

bool url_queue::pop(url& u) {
  std::lock_guard lock(_mutex);

  if (_queue.empty()) {
    return false;
  }

  u = std::move(_queue.front());
  _queue.pop();
  return true;
}

void url_queue::push(url&& u) {
  std::lock_guard lock(_mutex);
  _queue.emplace(std::move(u));
}

void url_queue::push(const url& u) {
  std::lock_guard lock(_mutex);
  _queue.emplace(u);
}

void url_queue::push(std::vector<url>&& urls) {
  std::lock_guard lock(_mutex);
  for (auto url : urls) {
    _queue.emplace(std::move(url));
  }
}
