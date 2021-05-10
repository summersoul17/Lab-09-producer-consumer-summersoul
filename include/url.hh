// Copyright 2020 lamp
// Created by lamp on 5/10/21.
//

#ifndef INCLUDE_URL_HH_
#define INCLUDE_URL_HH_

#include <string>

struct url {
  url() = default;
  url(std::string u, size_t depth)
      : _url_str(std::move(u)), _depth(depth){};

  std::string _url_str;
  size_t _depth{0};
};

#endif  // INCLUDE_URL_HH_
