// Copyright 2020 lamp
// Created by lamp on 17.04.2021.
//
#include <downloader.hh>



void downloader::print_match(std::string url, std::string& protocol, std::string& host, std::string& target) {
  std::smatch url_match_result;
  BOOST_LOG_TRIVIAL(debug) << "Checking: " << url;
  if (std::regex_match(url, url_match_result, _url_regex)) {
    protocol = url_match_result[2];
    BOOST_LOG_TRIVIAL(debug) << "Protocol: " << protocol;
    host = url_match_result[4];
    BOOST_LOG_TRIVIAL(debug) << "Host: " << host;
    target = url_match_result[5];
    BOOST_LOG_TRIVIAL(debug) << "Target: " << target;
  } else {
    BOOST_LOG_TRIVIAL(error) << "Malformed url";
  }
}
void downloader::load(const url& u) {
  std::string protocol{}, host{}, target{};
  print_match(u._url_str, protocol, host, target);
  std::string body{};

  try {
    if (protocol == "http"){
      load_http(body, std::move(host), std::move(target));
    } else if (protocol == "https"){
      load_https(body, std::move(host), std::move(target));
    }
  } catch (std::exception& e) {
    BOOST_LOG_TRIVIAL(error)
        << "Failed loading: " << u._url_str << "\nerror: " << e.what();
    return;
  }

  page_queue::get_instance()->push(page(u, body));
}
void downloader::load_http(std::string& body, std::string&& host, std::string&& target) {
  boost::asio::ip::tcp::resolver resolver(_context);
  boost::beast::tcp_stream stream(_context);

  auto const results = resolver.resolve(host, "80");

  stream.connect(results);
  stream.expires_after(std::chrono::seconds(3));

  boost::beast::http::request<boost::beast::http::string_body> req{
      boost::beast::http::verb::get, target, 11};
  req.set(boost::beast::http::field::host, host);
  req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

  boost::beast::http::write(stream, req);

  boost::beast::flat_buffer buffer;
  boost::beast::http::response<boost::beast::http::string_body> res;

  boost::beast::http::read(stream, buffer, res);

  boost::beast::error_code ec;
  stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

  body = res.body();
}
void downloader::load_https(std::string& body, std::string&& host, std::string&& target) {
  _ssl_context.set_default_verify_paths();
  _ssl_context.add_verify_path("/etc/ssl/certs/");
  _ssl_context.set_verify_mode(boost::asio::ssl::verify_peer);

  boost::asio::ip::tcp::resolver resolver(_context);
  boost::beast::ssl_stream<boost::beast::tcp_stream> stream(_context, _ssl_context);

  if (!SSL_set_tlsext_host_name(stream.native_handle(), host.data())) {
    boost::beast::error_code ec{static_cast<int>(::ERR_get_error()),
                                boost::asio::error::get_ssl_category()};
    throw boost::beast::system_error{ec};
  }

  auto const results = resolver.resolve(host, "443");

  boost::beast::get_lowest_layer(stream).connect(results);

  stream.handshake(boost::asio::ssl::stream_base::client);

  boost::beast::http::request<boost::beast::http::string_body> req{
      boost::beast::http::verb::get, target, 11};
  req.set(boost::beast::http::field::host, host);
  req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

  boost::beast::http::write(stream, req);

  boost::beast::flat_buffer buffer;
  boost::beast::http::response<boost::beast::http::string_body> res;

  boost::beast::http::read(stream, buffer, res);

  boost::beast::error_code ec;
  stream.shutdown(ec);

  body = res.body();
}
