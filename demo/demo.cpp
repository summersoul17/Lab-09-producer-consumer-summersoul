//
// Created by lamp on 13.04.2021.
//

#include <settings.hh>
#include <downloader.hh>
#include <iostream>

std::unique_ptr<settings> instance::_instance = nullptr;
std::mutex instance::_mutex;

int main(int argc, char** argv) {
  if (!instance::get_instance()->set_settings(
          instance::get_instance()->parse_command_line(argc, argv))) {
    BOOST_LOG_TRIVIAL(fatal) << "[ERROR]";
    return 1;
  }
  BOOST_LOG_TRIVIAL(trace) << instance::get_instance();
  downloader loader(instance::get_instance()->downloaders());
  loader.print_match(instance::get_instance()->url());
  BOOST_LOG_TRIVIAL(trace) << "[Final]";
  return 0;
}