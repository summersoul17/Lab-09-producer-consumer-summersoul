//
// Created by lamp on 13.04.2021.
//

#include <settings.hh>

#include <downloader.hh>
#include <iostream>

int main(int argc, char** argv) {
  settings* s = instance::get_instance();
  if (!s->set_settings(s->parse_command_line(argc, argv))) {
    BOOST_LOG_TRIVIAL(fatal) << "[ERROR]";
    return 1;
  }
  BOOST_LOG_TRIVIAL(trace) << "[Final]";
  return 0;
}