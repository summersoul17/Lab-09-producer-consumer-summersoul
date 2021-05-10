// Copyright 2020 lamp
//
// Created by lamp on 17.04.2021.
//

#ifndef INCLUDE_HTML_PARSER_HH_
#define INCLUDE_HTML_PARSER_HH_

#include <string>
#include <page.hh>

class html_parser {
 public:
  html_parser() = default;
  void parse(const page&);
  void set_depth(size_t depth) { _depth = depth; }

 private:
  size_t _depth;
};

#endif  // INCLUDE_HTML_PARSER_HH_
