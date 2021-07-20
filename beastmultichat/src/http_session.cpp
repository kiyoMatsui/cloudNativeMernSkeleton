//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/vinniefalco/CppCon2018
//

#include "http_session.hpp"
#include <algorithm>
#include <boost/config.hpp>
#include <iostream>

#include "websocket_session.hpp"

#define BOOST_NO_CXX14_GENERIC_LAMBDAS

//------------------------------------------------------------------------------

// Return a reasonable mime type based on the extension of a file.
std::string_view mime_type(std::string_view path) {
  auto const ext = [&path] {
    auto const pos = path.rfind(".");
    if (pos == std::string_view::npos) return std::string_view{};
    return path; //.substr(pos);
  }();
  if (ext.find(".htm") != std::string_view::npos) return "text/html";
  if (ext.find(".html") != std::string_view::npos) return "text/html";
  if (ext.find(".php") != std::string_view::npos) return "text/html";
  if (ext.find(".css") != std::string_view::npos) return "text/css";
  if (ext.find(".txt") != std::string_view::npos) return "text/plain";
  if (ext.find(".js") != std::string_view::npos) return "application/javascript";
  if (ext.find(".json") != std::string_view::npos) return "application/json";
  if (ext.find(".xml") != std::string_view::npos) return "application/xml";
  if (ext.find(".swf") != std::string_view::npos) return "application/x-shockwave-flash";
  if (ext.find(".flv") != std::string_view::npos) return "video/x-flv";
  if (ext.find(".png") != std::string_view::npos) return "image/png";
  if (ext.find(".jpe") != std::string_view::npos) return "image/jpeg";
  if (ext.find(".jpeg") != std::string_view::npos) return "image/jpeg";
  if (ext.find(".jpg") != std::string_view::npos) return "image/jpeg";
  if (ext.find(".gif") != std::string_view::npos) return "image/gif";
  if (ext.find(".bmp") != std::string_view::npos) return "image/bmp";
  if (ext.find(".ico") != std::string_view::npos) return "image/vnd.microsoft.icon";
  if (ext.find(".tiff") != std::string_view::npos) return "image/tiff";
  if (ext.find(".tif") != std::string_view::npos) return "image/tiff";
  if (ext.find(".svg") != std::string_view::npos) return "image/svg+xml";
  if (ext.find(".svgz") != std::string_view::npos) return "image/svg+xml";
  return "application/text";
}

// Append an HTTP rel-path to a local filesystem path.
// The returned path is normalized for the platform.
std::string path_cat(std::string_view base, std::string_view path) {
  if (base.empty()) return std::string(path);
  std::string result(base);
#ifdef BOOST_MSVC
  char constexpr path_separator = '\\';
  if (result.back() == path_separator) result.resize(result.size() - 1);
  result.append(path.data(), path.size());
  for (auto& c : result)
    if (c == '/') c = path_separator;
#else
  char constexpr path_separator = '/';
  if (result.back() == path_separator) result.resize(result.size() - 1);
  result.append(path.data(), path.size());
#endif
  return result;
}

// This function produces an HTTP response for the given
// request. The type of the response object depends on the
// contents of the request, so the interface requires the
// caller to pass a generic lambda for receiving the response.
template <class Body, class Allocator, class Send>
void handle_request(std::string_view doc_root, http::request<Body, http::basic_fields<Allocator>>&& req,
                    Send&& send) {
  // Returns a bad request response
  auto const bad_request = [&req](std::string_view why) {
    http::response<http::string_body> res{http::status::bad_request, req.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/html");
    res.keep_alive(req.keep_alive());
    res.body() = std::string(why);
    res.prepare_payload();
    return res;
  };

  // Returns a not found response
  auto const not_found = [&req](std::string_view target) {
    http::response<http::string_body> res{http::status::not_found, req.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/html");
    res.keep_alive(req.keep_alive());
    res.body() = "The resource '" + std::string(target) + "' was not found.";
    res.prepare_payload();
    return res;
  };

  // Returns a server error response
  auto const server_error = [&req](std::string_view what) {
    http::response<http::string_body> res{http::status::internal_server_error, req.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/html");
    res.keep_alive(req.keep_alive());
    res.body() = "An error occurred: '" + std::string(what) + "'";
    res.prepare_payload();
    return res;
  };

  // Make sure we can handle the method
  if (req.method() != http::verb::get && req.method() != http::verb::head)
    return send(bad_request("Unknown HTTP-method"));

  // Request path must be absolute and not contain "..".
  if (req.target().empty() || req.target()[0] != '/' || req.target().find("..") != std::string_view::npos)
    return send(bad_request("Illegal request-target"));

  // Build the path to the requested file
  std::string path = path_cat(doc_root, std::string_view(req.target().data(), req.target().size()));
  if (req.target().back() == '/') path.append("index.html");

  // Attempt to open the file
  beast::error_code ec;
  http::file_body::value_type body;
  body.open(path.c_str(), beast::file_mode::scan, ec);

  // Handle the case where the file doesn't exist
  if (ec == boost::system::errc::no_such_file_or_directory) return send(not_found(std::string_view(req.target().data(), req.target().size())));

  // Handle an unknown error
  if (ec) return send(server_error(ec.message()));

  // Cache the size since we need it after the move
  auto const size = body.size();

  // Respond to HEAD request
  if (req.method() == http::verb::head) {
    http::response<http::empty_body> res{http::status::ok, req.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, mime_type(path));
    res.content_length(size);
    res.keep_alive(req.keep_alive());
    return send(std::move(res));
  }

  // Respond to GET request
  http::response<http::file_body> res{std::piecewise_construct, std::make_tuple(std::move(body)),
                                      std::make_tuple(http::status::ok, req.version())};
  res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
  res.set(http::field::content_type, mime_type(path));
  res.content_length(size);
  res.keep_alive(req.keep_alive());
  return send(std::move(res));
}

//------------------------------------------------------------------------------

struct http_session::send_lambda {
  http_session& self_;

  explicit send_lambda(http_session& self) : self_(self) {}

  template <bool isRequest, class Body, class Fields>
  void operator()(http::message<isRequest, Body, Fields>&& msg) const {
    // The lifetime of the message has to extend
    // for the duration of the async operation so
    // we use a shared_ptr to manage it.
    auto sp = std::make_shared<http::message<isRequest, Body, Fields>>(std::move(msg));

    // Write the response
    auto self = self_.shared_from_this();
    http::async_write(self_.stream_, *sp, [self, sp](beast::error_code ec, std::size_t bytes) {
      self->on_write(ec, bytes, sp->need_eof());
    });
  }
};

//------------------------------------------------------------------------------

http_session::http_session(tcp::socket&& socket, std::shared_ptr<shared_state> const& state)
    : stream_(std::move(socket)), state_(state) {}

void http_session::run() {
  do_read();
}

// Report a failure
void http_session::fail(beast::error_code ec, char const* what) {
  // Don't report on canceled operations
  if (ec == net::error::operation_aborted) return;

  std::cerr << what << ": " << ec.message() << "\n";
}

void http_session::do_read() {
  // Construct a new parser for each message
  parser_.emplace();

  // Apply a reasonable limit to the allowed size
  // of the body in bytes to prevent abuse.
  parser_->body_limit(10000);

  // Set the timeout.
  stream_.expires_after(std::chrono::seconds(30));

  // Read a request
  http::async_read(stream_, buffer_, parser_->get(),
                   beast::bind_front_handler(&http_session::on_read, shared_from_this()));
}

void http_session::on_read(beast::error_code ec, std::size_t) {
  // This means they closed the connection
  if (ec == http::error::end_of_stream) {
    stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
    return;
  }

  // Handle the error, if any
  if (ec) return fail(ec, "read");

  // See if it is a WebSocket Upgrade
  if (websocket::is_upgrade(parser_->get())) {
    // Create a websocket session, transferring ownership
    // of both the socket and the HTTP request.
    std::make_shared<websocket_session>(stream_.release_socket(), state_)->run(parser_->release());
    return;
  }

  // Send the response
#ifndef BOOST_NO_CXX14_GENERIC_LAMBDAS
  //
  // The following code requires generic
  // lambdas, available in C++14 and later.
  //
  handle_request(state_->doc_root(), std::move(req_), [this](auto&& response) {
    // The lifetime of the message has to extend
    // for the duration of the async operation so
    // we use a shared_ptr to manage it.
    using response_type = typename std::decay<decltype(response)>::type;
    auto sp = std::make_shared<response_type>(std::forward<decltype(response)>(response));

#if 0
            // NOTE This causes an ICE in gcc 7.3
            // Write the response
            http::async_write(this->stream_, *sp,
                [self = shared_from_this(), sp](
                    beast::error_code ec, std::size_t bytes)
                {
                    self->on_write(ec, bytes, sp->need_eof()); 
                });
#else
            // Write the response
            auto self = shared_from_this();
            http::async_write(stream_, *sp,
                [self, sp](
                    beast::error_code ec, std::size_t bytes)
                {
                    self->on_write(ec, bytes, sp->need_eof()); 
                });
#endif
  });
#else
  //
  // This code uses the function object type send_lambda in
  // place of a generic lambda which is not available in C++11
  //
  handle_request(state_->doc_root(), parser_->release(), send_lambda(*this));

#endif
}

void http_session::on_write(beast::error_code ec, std::size_t, bool close) {
  // Handle the error, if any
  if (ec) return fail(ec, "write");

  if (close) {
    // This means we should close the connection, usually because
    // the response indicated the "Connection: close" semantic.
    stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
    return;
  }

  // Read another request
  do_read();
}
