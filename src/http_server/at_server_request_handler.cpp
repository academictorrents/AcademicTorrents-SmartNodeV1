//
// request_handler.cpp
//

#include "at_server_request_handler.hpp"

namespace http {
namespace at_server {

//extern Database *db;

request_handler::request_handler(const std::string& doc_root) :
		doc_root_(doc_root) {
}

void request_handler::handle_request(const request& req, reply& rep) {
	// Decode url to path.
	std::string request_path;
//  static const boost::regex collection_i("?=", boost::regex::icase);
	static const boost::regex log_file("^/log/?$",
			boost::regex::icase);
	static const boost::regex collections("^/collections/?$",
			boost::regex::icase);
	static const boost::regex collection_info("^/collections/[a-z,0-9,\-]+/?$",
			boost::regex::icase);
	static const boost::regex collection_data("^/data/[a-z,0-9,\-]*.*/?$",
			boost::regex::icase);
	static const boost::regex collection_subscribe(
			"^/subscribe/[a-z,0-9,\-]+/?$", boost::regex::icase);
	static const boost::regex collection_unsubscribe(
			"^/unsubscribe/[a-z,0-9,\-]+/?$", boost::regex::icase);

	if (!url_decode(req.uri, request_path)) {
		rep = reply::stock_reply(reply::bad_request);
		return;
	}

	// Request path must be absolute and not contain "..".
	if (request_path.empty() || request_path[0] != '/'
			|| request_path.find("..") != std::string::npos) {
		rep = reply::stock_reply(reply::bad_request);
		return;
	}

	// If path ends in slash (i.e. is a directory) then add "index.html".
	if (request_path.size() == 1
			&& request_path[request_path.size() - 1] == '/') {
		request_path += "index.html";
	}

	if(boost::regex_search(request_path, log_file)){
	  stringstream content;
	  string line;
	  ifstream myfile ("smartnode_log.log");
	  
	  if (myfile.is_open()){
	    while ( getline (myfile,line) )
	      {
		content << line << "\n";
	      }
	    myfile.close();
	    
	  rep.status = reply::ok;
	  rep.content.append(content.str());
	  rep.headers.resize(2);
	  rep.headers[0].name = "Content-Length";
	  rep.headers[0].value = boost::lexical_cast < std::string
						       > (rep.content.size());
	  rep.headers[1].name = "Content-Type";
	  rep.headers[1].value = mime_types::extension_to_type("text");
	  
	  return;

	  } 
	  else {
	    rep.status = reply::not_found;
	    rep.content.append("Unable to open file");
	    rep.headers.resize(2);
	    rep.headers[0].name = "Content-Length";
	    rep.headers[0].value = boost::lexical_cast < std::string
							 > (rep.content.size());
	    rep.headers[1].name = "Content-Type";
	    rep.headers[1].value = mime_types::extension_to_type("text");
	    
	    return;
	  } 
	}

	if (boost::regex_search(request_path, collections)) {
		boost::property_tree::ptree pt;
		boost::property_tree::ptree children;
		boost::property_tree::ptree child;

		Database *db1 = new Database();
		db1->open(DATABASE_NAME);
		vector < vector<string> > results =
				db1->query(
						"Select C.name, C.urlname, C.torrentcount, C.totalsizebytes, C.mirrored from Collections C;");
		std::vector < string > col = db1->getColNames();
		db1->close();

		for (std::vector<vector<string> >::iterator it_outer = results.begin();
				it_outer != results.end(); ++it_outer) {
			vector < string > vec = *it_outer;
			for (int i = 0; i < col.size(); i++) { 
			  child.put(col[i], vec[i]);
			}
			children.push_back(std::make_pair("", child));
		}

		pt.add_child("Collections", children);
		std::stringstream json;
		boost::property_tree::json_parser::write_json(json, pt);

		rep.status = reply::ok;
		rep.content.append(json.str());
		rep.headers.resize(2);
		rep.headers[0].name = "Content-Length";
		rep.headers[0].value = boost::lexical_cast < std::string
				> (rep.content.size());
		rep.headers[1].name = "Content-Type";
		rep.headers[1].value = mime_types::extension_to_type("json");

		return;
	}

	if (boost::regex_search(request_path, collection_info)) {
		std::string collection_id;
		std::vector < string > splits;
		boost::split(splits, request_path, boost::is_any_of("/"));
		if (splits[splits.size() - 1] != "") {
			collection_id = splits[splits.size() - 1];
		} else {
			collection_id = splits[splits.size() - 2];
		}

		boost::property_tree::ptree pt;
		boost::property_tree::ptree children;
		boost::property_tree::ptree child;

		Database *db1 = new Database();
		db1->open(DATABASE_NAME);
		vector < vector<string> > results =
				db1->query(
						"SELECT DISTINCT T.type, T.name, T.infohash, T.sizebytes, T.mirrors, T.downloaders, T.timescompleted, T.dateadded, T.datemodified, T.status, T.filename, T.bibtex FROM torrents T JOIN Collections2Torrents C ON T.infohash=C.infohash WHERE C.urlname=\""
								+ collection_id + "\";");
		std::vector < string > col = db1->getColNames();
		db1->close();
		string line;
		for (int i = 0; i < results.size(); i++) {
			vector < string > inner = results[i];
			for (int j = 0; j < col.size(); j++) {
			  line = inner[j];
			  boost::replace_all(line, "\"", ""); 
			  child.put(col[j], line);
				child.put(col[j], line);
			}
			children.push_back(std::make_pair("", child));
		}

		pt.add_child(collection_id, children);
		std::stringstream json;
		boost::property_tree::json_parser::write_json(json, pt);

		rep.status = reply::ok;
		rep.content.append(json.str());
		rep.headers.resize(2);
		rep.headers[0].name = "Content-Length";
		rep.headers[0].value = boost::lexical_cast < std::string
				> (rep.content.size());
		rep.headers[1].name = "Content-Type";
		rep.headers[1].value = mime_types::extension_to_type("json");

		return;
	}

	if (boost::regex_search(request_path, collection_subscribe)) {
		std::string collection_id;
		std::vector < string > splits;
		boost::split(splits, request_path, boost::is_any_of("/"));
		if (splits[splits.size() - 1] != "") {
			collection_id = splits[splits.size() - 1];
		} else {
			collection_id = splits[splits.size() - 2];
		}
		query_str qr;
		string com =
				"UPDATE Torrents SET status=2 WHERE infohash IN (select C.infohash FROM Collections2Torrents C where C.urlname=\'"
						+ collection_id + "\');";
		qr.command = com;
		qr.priority = 99;
		db->addquery(&qr);

		query_str qr2;
		string com2 =
				"UPDATE Collections SET mirrored=1 WHERE urlname=\'" + collection_id + "\';";
		qr2.command = com2;
		qr2.priority = 99;
		db->addquery(&qr2);

		rep.status = reply::ok;
		rep.content.append(collection_id);
		rep.headers.resize(2);
		rep.headers[0].name = "Content-Length";
		rep.headers[0].value = boost::lexical_cast < std::string
				> (rep.content.size());
		rep.headers[1].name = "Content-Type";
		rep.headers[1].value = mime_types::extension_to_type("html");

		return;
	}

	if (boost::regex_search(request_path, collection_unsubscribe)) {
		std::string collection_id;
		std::vector < string > splits;
		boost::split(splits, request_path, boost::is_any_of("/"));
		if (splits[splits.size() - 1] != "") {
			collection_id = splits[splits.size() - 1];
		} else {
			collection_id = splits[splits.size() - 2];
		}
			
		Database *db_unsub = new Database();
		db_unsub->open(DATABASE_NAME);
		vector<vector<string> > results = db_unsub->query("SELECT C.infohash FROM Collections2Torrents C WHERE C.urlname=\'" + collection_id + "\');");
		vector<vector<string> > results_2;
	
		query_str qr;
		string com =
				"UPDATE Torrents SET status=6 WHERE infohash IN (select C.infohash FROM Collections2Torrents C where C.urlname=\'"
						+ collection_id + "\');";
		qr.command = com;
		qr.priority = 99;
		db->addquery(&qr);

		query_str qr2;
		string com2 =
				"UPDATE Collections SET mirrored=0 WHERE urlname=\'"
						+ collection_id + "\';";
		qr.command = com2;
		qr2.priority = 99;
		db->addquery(&qr2);

		rep.status = reply::ok;
		rep.content.append(collection_id);
		rep.headers.resize(2);
		rep.headers[0].name = "Content-Length";
		rep.headers[0].value = boost::lexical_cast < std::string
				> (rep.content.size());
		rep.headers[1].name = "Content-Type";
		rep.headers[1].value = mime_types::extension_to_type("html");

		return;
	}

	if (boost::regex_search(request_path, collection_data)) {
		std::string torrent_path = "./data/";
		std::vector < string > splits;

		boost::split(splits, request_path, boost::is_any_of("/"));
		if (splits[splits.size() - 1] != "") {
		        torrent_path += splits[splits.size() - 1];
			torrent_path += splits[splits.size() - 2];
		} else {
		        torrent_path += splits[splits.size() - 2];
			torrent_path += splits[splits.size() - 3];
		}
/*
          stringstream content;
	  string line;
          content << torrent_path;
	  fstream myfile( content.str());
	  
	  if (myfile.is_open()){
	    while ( getline (myfile,line) )
	      {
		content << line << "\n";
	      }
	    myfile.close();
	    
	  rep.status = reply::ok;
	  rep.content.append(content.str());
	  rep.headers.resize(2);
	  rep.headers[0].name = "Content-Length";
	  rep.headers[0].value = boost::lexical_cast < std::string
						       > (rep.content.size());
	  rep.headers[1].name = "Content-Type";
	  rep.headers[1].value = mime_types::extension_to_type("pdf");
	  
	  return;

	  } */
			       

		// Database *db1 = new Database();
		// db1->open(DATABASE_NAME);
		// vector < vector<string> > results = db1->query(
		// 		"Select T.filename FROM Torrents T WHERE T.infohash="
		// 				+ torrent_hash + ";");
		// db1->close();

		// for (int i = 0; i < results.size(); i++) {
		// 	vector < string > inner = results[i];
		// 	for (int j = 0; j < inner.size(); j++) {
		// 		torrent_name = inner[j];
		// 	}
		// }
		/*
		fs::path someDir("./data/"+ torrent_name);
		fs::directory_iterator end_iter;

		if ( fs::exists(someDir) && fs::is_directory(someDir))
		  {
		    for( fs::directory_iterator dir_iter(someDir) ; dir_iter != end_iter ; ++dir_iter)
		      {
			if (fs::is_regular_file(dir_iter->status()) )
			  {
			    content << fs::last_write_time(dir_iter->status()), *dir_iter);
					      
			  }
		      }
	           }
	*/
		
		
	/*
		rep.status = reply::ok;
		rep.content.append("");
		rep.headers.resize(2);
		rep.headers[0].name = "Content-Length";
		rep.headers[0].value = boost::lexical_cast < std::string
				> (rep.content.size());
		rep.headers[1].name = "Content-Type";
		rep.headers[1].value = mime_types::extension_to_type("pdf");
	
		return;*/
	}
	
	// Determine the file extension.
	std::size_t last_slash_pos = request_path.find_last_of("/");
	std::size_t last_dot_pos = request_path.find_last_of(".");
	std::string extension;
	if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
		extension = request_path.substr(last_dot_pos + 1);
	}

	// Open the file to send back.
	std::string full_path = doc_root_ + request_path;
	std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
	if (!is) {
		full_path = "./" + request_path;
		is.open(full_path.c_str(), std::ios::in | std::ios::binary);
		if(!is){
			rep = reply::stock_reply(reply::not_found);
		return;
		}
	}

	// Fill out the reply to be sent to the client.
	rep.status = reply::ok;
	char buf[512];
	while (is.read(buf, sizeof(buf)).gcount() > 0)
		rep.content.append(buf, is.gcount());
	rep.headers.resize(2);
	rep.headers[0].name = "Content-Length";
	rep.headers[0].value = boost::lexical_cast < std::string
			> (rep.content.size());
	rep.headers[1].name = "Content-Type";
	rep.headers[1].value = mime_types::extension_to_type(extension);
	
}

bool request_handler::url_decode(const std::string& in, std::string& out) {
	out.clear();
	out.reserve(in.size());
	for (std::size_t i = 0; i < in.size(); ++i) {
		if (in[i] == '%') {
			if (i + 3 <= in.size()) {
				int value = 0;
				std::istringstream is(in.substr(i + 1, 2));
				if (is >> std::hex >> value) {
					out += static_cast<char>(value);
					i += 2;
				} else {
					return false;
				}
			} else {
				return false;
			}
		} else if (in[i] == '+') {
			out += ' ';
		} else {
			out += in[i];
		}
	}
	return true;
}

} // namespace at_server
} // namespace http
