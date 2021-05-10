// Copyright 2020 lamp
// Created by lamp on 17.04.2021.
//

#ifndef INCLUDE_SETTINGS_HH_
#define INCLUDE_SETTINGS_HH_

#include <boost/program_options.hpp>
#include <log_setup.hh>
namespace po = boost::program_options;

#include <mutex>
#include <memory>
#include <string>

class settings {
 public:
  static settings* get_instance();
  po::variables_map parse_command_line(int argc, char** argv);
  bool set_settings(const po::variables_map& vm);

  settings(const settings&) = delete;
  settings& operator=(const settings&) = delete;
  settings(settings&&) = delete;
  settings& operator=(settings&&) = delete;

  [[nodiscard]] auto url() const -> const std::string& { return _url; }
  [[nodiscard]] auto depth() const -> size_t { return _depth; }
  [[nodiscard]] auto downloaders() const -> size_t { return _downloaders; }
  [[nodiscard]] auto parsers() const -> size_t { return _parsers; }
  [[nodiscard]] auto output() const -> const std::string& { return _output; }

  friend std::ostream& operator<<(std::ostream& os, const settings& s) {
    os << "Settings:\n\tUrl: " << s._url << "\n\tDepth: " << s._depth
       << "\n\tDownloaders: " << s._downloaders << "\n\tParsers: " << s._parsers
       << "\n\tOutput file: " << s._output << '\n';
    return os;
  }

 private:
  settings();
  po::options_description _desc{"Allowed options"};
  std::string _url{};
  size_t _depth = 1;
  size_t _downloaders = 1;
  size_t _parsers = 1;
  std::string _output = "output.txt";
};

#endif  // INCLUDE_SETTINGS_HH_
