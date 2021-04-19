//
// Created by lamp on 17.04.2021.
//

#ifndef CRAWLER_SETTINGS_HH
#define CRAWLER_SETTINGS_HH

#include <boost/program_options.hpp>
#include <log_setup.hh>
namespace po = boost::program_options;

#include <mutex>
#include <memory>

class settings {
 public:
  friend std::ostream& operator<<(std::ostream& os, const std::unique_ptr<settings>& s);
  po::variables_map parse_command_line(int argc, char** argv);
  bool set_settings(const po::variables_map& vm);
  static settings* create_instance();

  settings(const settings&) = delete;
  settings& operator=(const settings&) = delete;

  [[nodiscard]] auto url() const -> const std::string& { return _url; }
  [[nodiscard]] auto depth() const -> size_t { return _depth; }
  [[nodiscard]] auto downloaders() const -> size_t { return _downloaders; }
  [[nodiscard]] auto parsers() const -> size_t { return _parsers; }
  [[nodiscard]] auto output() const -> const std::string& { return _output; }
 private:
  settings();
  po::options_description _desc{"Allowed options"};
  std::string _url{};
  size_t _depth = 1;
  size_t _downloaders = 1;
  size_t _parsers = 1;
  std::string _output = "output.txt";
};

class instance {
 public:
  instance() = delete;
  static std::unique_ptr<settings>& get_instance() {
    std::unique_lock<std::mutex> lock(_mutex);
    return (_instance) ? _instance : create_instance();
  }
 private:
  static std::unique_ptr<settings>& create_instance() {
    _instance.reset(settings::create_instance());
    return _instance;
  }
 private:
  static std::mutex _mutex;
  static std::unique_ptr<settings> _instance;
};



#endif  // CRAWLER_SETTINGS_HH
