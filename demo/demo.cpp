// Copyright 2020 lamp
// Created by lamp on 13.04.2021.
//

#include <html_parser.hh>
#include <settings.hh>
#include <downloader.hh>
#include <iostream>

std::regex downloader::_url_regex{
    R"(^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)",
    std::regex::extended
};



auto downloader_func(){
  downloader loader{};

  url u{};

  const size_t fail_limit = 5;
  size_t fails = 0;
  while(fails < fail_limit){
    if (url_queue::get_instance()->pop(u)){
      fails = 0;
      loader.load(u);
    } else {
      ++fails;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }
}

auto parser_func(){
  html_parser parser{};

  parser.set_depth(settings::get_instance()->depth());

  page p{};

  const size_t fail_limit = 5;
  size_t fails = 0;
  while(fails < fail_limit){
    if (page_queue::get_instance()->pop(p)){
      fails = 0;
      parser.parse(p);
    } else {
      ++fails;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }
}

auto writer_func(){
  const size_t fail_limit = 10;
  size_t fails = 0;
  while(fails < fail_limit){
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if (image_url_queue::get_instance()->dump()){
      fails = 0;
      BOOST_LOG_TRIVIAL(debug) << "Dumped";
    } else {
      ++fails;
    }
  }
}

int main(int argc, char** argv) {
  auto options = settings::get_instance();
  if (!options->set_settings(options->parse_command_line(argc, argv))) {
    BOOST_LOG_TRIVIAL(fatal) << "[ERROR]";
    return 1;
  }
  BOOST_LOG_TRIVIAL(info) << *options;
  image_url_queue::get_instance()->update_filename(options->output());

  url starting {options->url(), 0};
  url_queue::get_instance()->push(std::move(starting));

  ThreadPool downloaders_pool{options->downloaders()};
  ThreadPool parsers_pool{options->parsers()};

  for (size_t i = 0; i < options->downloaders(); ++i){
    downloaders_pool.enqueue(downloader_func);
  }

  for (size_t i = 0; i < options->parsers(); ++i){
    parsers_pool.enqueue(parser_func);
  }

  std::thread(writer_func).join(); // writer thread

  return 0;
}