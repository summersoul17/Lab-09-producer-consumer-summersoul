// Copyright 2020 lamp
// Created by lamp on 17.04.2021.
//

#ifndef INCLUDE_DOWNLOADER_HH_
#define INCLUDE_DOWNLOADER_HH_

#include <regex>

#include <url_queue.hh>
#include <page_queue.hh>
#include <image_url_queue.hh>
#include <settings.hh>
#include <log_setup.hh>

#include <iostream>
#include <string>

// Boost libs
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/ssl.hpp>

namespace http = boost::beast::http;
namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

#include <ThreadPool.h>

class downloader {
 public:
  downloader()
      : _context{1},
        _ssl_context(ssl::context::tlsv12_client) {}
  downloader(const downloader&) = delete;
  downloader& operator=(const downloader&) = delete;

  static void print_match(std::string, std::string&, std::string&,
                   std::string&);  // Debug function only
  void load(const url&);
  void load_http(std::string&, std::string&&, std::string&&);
  void load_https(std::string&, std::string&&, std::string&&);

 private:
  static std::regex _url_regex;
  net::io_context _context;
  ssl::context _ssl_context;
};

#endif  // INCLUDE_DOWNLOADER_HH_
