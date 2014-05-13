//
// request_handler.hpp
// ~~~~~~~~~~~~~~~~~~~
//

#ifndef HTTP_AT_SERVER_REQUEST_HANDLER_HPP
#define HTTP_AT_SERVER_REQUEST_HANDLER_HPP

#include <string>

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include "at_server_mime_types.hpp"
#include "at_server_reply.hpp"
#include "at_server_request.hpp"
#include "database.hpp"
//#include "SmartNode.hpp"

using boost::property_tree::ptree;

namespace http {
namespace at_server {

struct reply;
struct request;

/// The common handler for all incoming requests.
class request_handler
  : private boost::noncopyable
{
public:
  /// Construct with a directory containing files to be served.
  explicit request_handler(const std::string& doc_root);

  /// Handle a request and produce a reply.
  void handle_request(const request& req, reply& rep);

private:
  /// The directory containing the files to be served.
  std::string doc_root_;

  /// Perform URL-decoding on a string. Returns false if the encoding was
  /// invalid.
  static bool url_decode(const std::string& in, std::string& out);

  std::string createJson(std::vector<vector<string> >);
};

} // namespace at_server
} // namespace http

#endif // HTTP_AT_SERVER_REQUEST_HANDLER_HPP
