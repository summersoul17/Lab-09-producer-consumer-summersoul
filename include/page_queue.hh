// Copyright 2020 lamp
// Created by lamp on 5/10/21.
//

#ifndef INCLUDE_PAGE_QUEUE_HH_
#define INCLUDE_PAGE_QUEUE_HH_

#include <page.hh>

#include <vector>
#include <queue>
#include <mutex>

class page_queue {
 public:
  static page_queue* get_instance();

  bool pop(page&);
  void push(const page&);
  void push(page&&);

  page_queue(const page_queue&) = delete;
  page_queue(page_queue&&) = delete;
  page_queue& operator=(const page_queue&) = delete;
  page_queue& operator=(page_queue&) = delete;

 private:
  page_queue() = default;

  mutable std::mutex _mutex;
  std::queue<page> _queue;
};

#endif  // INCLUDE_PAGE_QUEUE_HH_
