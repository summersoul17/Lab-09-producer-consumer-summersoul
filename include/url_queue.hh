// Copyright 2020 lamp
// Created by lamp on 5/10/21.
//

#ifndef INCLUDE_URL_QUEUE_HH_
#define INCLUDE_URL_QUEUE_HH_

#include <url.hh>

#include <string_view>
#include <string>
#include <vector>
#include <queue>
#include <mutex>

class url_queue {
 public:
  static url_queue* get_instance();

  bool pop(url&);
  void push(const url&);
  void push(url&&);
  void push(std::vector<url>&&);

  url_queue(const url_queue&) = delete;
  url_queue(url_queue&&) = delete;
  url_queue& operator=(const url_queue&) = delete;
  url_queue& operator=(url_queue&) = delete;

 private:
  url_queue() = default;

  mutable std::mutex _mutex;
  std::queue<url> _queue;
};

#endif  // INCLUDE_URL_QUEUE_HH_
