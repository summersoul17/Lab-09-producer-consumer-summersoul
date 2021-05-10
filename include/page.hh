// Copyright 2020 lamp
// Created by lamp on 5/10/21.
//

#ifndef INCLUDE_PAGE_HH_
#define INCLUDE_PAGE_HH_

#include <string>
#include <url.hh>
#include <utility>

struct page {
  page() = default;
  page(std::string url, size_t depth, std::string body)
      : url_{std::move(url), depth}, body_(std::move(body)) {}
  page(url u, std::string body) : url_(std::move(u)), body_(std::move(body)) {}

  url url_;
  std::string body_;
};

#endif  // INCLUDE_PAGE_HH_
