//
// Created by lamp on 17.04.2021.
//
#include <downloader.hh>

downloader::downloader(unsigned pool_capacity)
    :_pool{pool_capacity}, _context{1},
_ssl_context(ssl::context::tlsv12_client){
  load_root_certificates(_ssl_context);
  ctx.set_verify_mode(ssl::verify_peer);
}

void downloader::print_match(const std::string & url) {
  std::smatch url_match_result;
  BOOST_LOG_TRIVIAL(debug) << "Checking: " << url;
  if (std::regex_match(url, url_match_result, _url_regex)){
    BOOST_LOG_TRIVIAL(debug) << "Protocol: " << url_match_result[2];
    BOOST_LOG_TRIVIAL(debug) << "Host: " << url_match_result[4];
    BOOST_LOG_TRIVIAL(debug) << "Target: " << url_match_result[5];
  } else {
    BOOST_LOG_TRIVIAL(error) << "Malformed url";
  }
}
html_page downloader::load(const std::string& url) {
  tcp::resolver resolver(_context);
  beast::ssl_stream<beast::tcp_stream> stream(_context, _ssl_context);
}
