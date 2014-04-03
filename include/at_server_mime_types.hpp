//
// mime_types.hpp
// ~~~~~~~~~~~~~~
//

#ifndef HTTP_AT_SERVER_MIME_TYPES_HPP
#define HTTP_AT_SERVER_MIME_TYPES_HPP

#include <string>

namespace http {
namespace at_server {
namespace mime_types {

/// Convert a file extension into a MIME type.
std::string extension_to_type(const std::string& extension);

} // namespace mime_types
} // namespace at_server
} // namespace http

#endif // HTTP_AT_SERVER_MIME_TYPES_HPP
