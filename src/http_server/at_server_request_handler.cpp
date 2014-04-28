//
// request_handler.cpp
//

#include "at_server_request_handler.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include "at_server_mime_types.hpp"
#include "at_server_reply.hpp"
#include "at_server_request.hpp"

namespace http {
namespace at_server {

request_handler::request_handler(const std::string& doc_root)
  : doc_root_(doc_root)
{
}

void request_handler::handle_request(const request& req, reply& rep)
{
  // Decode url to path.
  std::string request_path;
  if (!url_decode(req.uri, request_path))
  {
    rep = reply::stock_reply(reply::bad_request);
    return;
  }

  // Request path must be absolute and not contain "..".
  if (request_path.empty() || request_path[0] != '/'
      || request_path.find("..") != std::string::npos)
  {
    rep = reply::stock_reply(reply::bad_request);
    return;
  }

  std::cout << request_path;

  // If path ends in slash (i.e. is a directory) then add "index.html".
  if (request_path[request_path.size() - 1] == '/')
  {

    request_path += "index.html";
  }
  if(request_path == "/collections"){
	  boost::property_tree::ptree pt;
	  boost::property_tree::ptree children;
	  boost::property_tree::ptree child;

	  std::vector<string> col;
		col.push_back("name");
		col.push_back("urlname");
		col.push_back("torrent_count");
		col.push_back("size");

	  Database *db = new Database(DATABASE_NAME);
	  vector<vector<string> > results = db->query("Select * from Collections;");

	  for(std::vector<vector<string> >::iterator it_outer = results.begin(); it_outer != results.end(); ++it_outer){
		  vector<string> vec = *it_outer;
		  for(int i = 0; i < 4; i++){
			  child.put(col[i], vec[i]);
		  }
		  children.push_back(std::make_pair("", child));
	  }

	  pt.add_child("Collections", children);
	  std::stringstream json;
	  boost::property_tree::json_parser::write_json(json, pt);

	  rep.status = reply::ok;
//	  rep.content.append(createJson(results));
	  rep.content.append(json.str());
	  	  rep.headers.resize(2);
	    rep.headers[0].name = "Content-Length";
	    rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
	    rep.headers[1].name = "Content-Type";
	    rep.headers[1].value = mime_types::extension_to_type("json");
	    return;


  }



  // Determine the file extension.
  std::size_t last_slash_pos = request_path.find_last_of("/");
  std::size_t last_dot_pos = request_path.find_last_of(".");
  std::string extension;
  if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
  {
    extension = request_path.substr(last_dot_pos + 1);
  }

  // Open the file to send back.
  std::string full_path = doc_root_ + request_path;
  std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
  if (!is)
  {
    rep = reply::stock_reply(reply::not_found);
    return;
  }

  // Fill out the reply to be sent to the client.
  rep.status = reply::ok;
  char buf[512];
  while (is.read(buf, sizeof(buf)).gcount() > 0)
    rep.content.append(buf, is.gcount());
  rep.headers.resize(2);
  rep.headers[0].name = "Content-Length";
  rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
  rep.headers[1].name = "Content-Type";
  rep.headers[1].value = mime_types::extension_to_type(extension);
}

std::string request_handler::createJson(std::vector<vector<string> > results){
	int i = 0;
	std::vector<string> col;
	col.push_back("name");
	col.push_back("urlname");
	col.push_back("torrent_count");
	col.push_back("size");
	string s = "{ \"Collections\": [";
	for(std::vector<vector<string> >::iterator it_outer = results.begin(); it_outer != results.end(); ++it_outer){
					i = 0;
			  	  	s.append("{ ");
		  			vector<string> vec = *it_outer;
		  			for(std::vector<string>::iterator it_inner = vec.begin(); it_inner != vec.end(); ++it_inner){
		  				s.append("\"" + col[0] + "\"" + ": " + "\"" + *it_inner + "\"}");
		  				i++;
		  			}
		  			s.append("}");
		  		}
	s.append("] }");
	return s;
}

bool request_handler::url_decode(const std::string& in, std::string& out)
{
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i)
  {
    if (in[i] == '%')
    {
      if (i + 3 <= in.size())
      {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value)
        {
          out += static_cast<char>(value);
          i += 2;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else if (in[i] == '+')
    {
      out += ' ';
    }
    else
    {
      out += in[i];
    }
  }
  return true;
}

} // namespace at_server
} // namespace http
