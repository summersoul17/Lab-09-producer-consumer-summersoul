//
// Created by lamp on 17.04.2021.
//

#include <settings.hh>

settings* settings::create_instance() {
  return new settings();
}

settings::settings() {
  _desc.add_options()
      ("help,h", "Shows help message")
      ("url,u", po::value<std::string>(), "Sets url to parse")
      ("depth,d", po::value<int>(), "Sets parsing depth. Default: 1")
      ("loaders,l", po::value<int>(), "Sets loaders count. Default: 1")
      ("parsers,p", po::value<int>(), "Sets parsers count. Default: 1")
      ("output,o", po::value<std::string>(), "Sets output file");
}

po::variables_map settings::parse_command_line(int argc, char **argv) {
  po::variables_map vm;
  po::parsed_options parsed = po::command_line_parser(argc, argv)
      .options(_desc)
      .allow_unregistered()
      .run();
  po::store(parsed, vm);
  po::notify(vm);
  return vm;
}

bool settings::set_settings(const po::variables_map &vm) {
  try {
    if (!vm.count("url")){
      throw std::runtime_error{"Not found input URL"};
    }
    _url = vm["url"].as<std::string>();
    if (vm.count("depth")){
      _depth = vm["depth"].as<int>();
    }
    if (vm.count("loaders")){
      _downloaders = vm["loaders"].as<int>();
    }
    if (vm.count("parsers")){
      _parsers = vm["parsers"].as<int>();
    }
    if (vm.count("output")){
      _output = vm["output"].as<std::string>();
    }

    return true;
  } catch (const std::exception& e){
    BOOST_LOG_TRIVIAL(fatal) << "Console parsing error: " << e.what()
        << "\nUsage:\n" << _desc;
    return false;
  }
}
std::ostream& operator<<(std::ostream& os, const std::unique_ptr<settings>& s) {
  os << "Settings:\n\tUrl: " << s->_url << "\n\tDepth: " << s->_depth
     << "\n\tDownloaders: " << s->_downloaders << "\n\tParsers: " << s->_parsers
     << "\n\tOutput file: " << s->_output << '\n';
  return os;
}
