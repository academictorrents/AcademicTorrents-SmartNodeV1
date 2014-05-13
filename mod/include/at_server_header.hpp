//
// header.hpp
// ~~~~~~~~~~
//

#ifndef HTTP_AT_SERVER_HEADER_HPP
#define HTTP_AT_SERVER_HEADER_HPP

#include <string>

namespace http {
namespace at_server {

struct header
{
  std::string name;
  std::string value;
};

} // namespace at_server
} // namespace http

#endif // HTTP_AT_SERVER_HEADER_HPP
