//
// Created by lamp on 17.04.2021.
//

#ifndef CRAWLER_DOWNLOADER_HH
#define CRAWLER_DOWNLOADER_HH

#include <ThreadPool.h>
#include <regex>
#include <settings.hh>
#include <log_setup.hh>
#include <root_cert.hpp>

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

namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

struct html_page{
  std::string url_{};
  std::string page_content_{};
}; // ??

class downloader {
 public:
  explicit downloader(unsigned pool_capacity);
  downloader(const downloader&) = delete;
  downloader& operator=(const downloader&) = delete;

  void print_match(const std::string&); // Debug function only
  html_page load(const std::string&);
  html_page load_http();
  html_page load_https();


 private:
  ThreadPool _pool;
  std::regex _url_regex{
      R"(^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)",
      std::regex::extended};
  net::io_context _context;
  ssl::context _ssl_context;
};

#endif  // CRAWLER_DOWNLOADER_HH
