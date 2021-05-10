//
// Created by lamp on 17.04.2021.
//

#ifndef CRAWLER_PARSER_HH
#define CRAWLER_PARSER_HH

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

#endif  // CRAWLER_PARSER_HH
