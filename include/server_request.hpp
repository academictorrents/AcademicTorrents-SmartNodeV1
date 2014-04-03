//
// request.hpp
//

#ifndef HTTP_AT_SERVER_REQUEST_HPP
#define HTTP_AT_SERVER_REQUEST_HPP

#include <string>
#include <vector>
#include "server_header.hpp"

namespace http {
namespace at_server {

/// A request received from a client.
struct request
{
  std::string method;
  std::string uri;
  int http_version_major;
  int http_version_minor;
  std::vector<header> headers;
};

} // namespace at_server
} // namespace http

#endif // HTTP_AT_SERVER_REQUEST_HPP
