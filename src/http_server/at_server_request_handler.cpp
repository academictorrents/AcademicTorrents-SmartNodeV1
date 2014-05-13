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
	static const boost::regex collections("^/collections/?$",
			boost::regex::icase);
	static const boost::regex collection_info("^/collections/[a-z,0-9,\-]+/?$",
			boost::regex::icase);
	static const boost::regex collection_data("^/data/[a-z,0-9,\-]+/?$",
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
			for (int i = 0; i < 5; i++) {
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
//		  boost::regex_search(request_path, collection_id, collection_i);

		boost::property_tree::ptree pt;
		boost::property_tree::ptree children;
		boost::property_tree::ptree child;

//		  cout << std::string(collection_id[0].first, collection_id[0].second);

		Database *db1 = new Database();
		db1->open(DATABASE_NAME);
		vector < vector<string> > results =
				db1->query(
						"SELECT DISTINCT T.type, T.name, T.infohash, T.sizebytes, T.mirrors, T.downloaders, T.timescompleted, T.dateadded, T.datemodified, T.status, T.filename, T.bibtex FROM torrents T JOIN Collections2Torrents C ON T.infohash=C.infohash WHERE C.urlname=\""
								+ collection_id + "\";");
		std::vector < string > col = db1->getColNames();
		db1->close();
		for (int i = 0; i < results.size(); i++) {
			vector < string > inner = results[i];
			for (int j = 0; j < col.size(); j++) {
				child.put(col[j], inner[j]);
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
		qr.priority = 9;
		db->addquery(&qr);

		query_str qr2;
		string com2 =
				"UPDATE Collections SET mirrored=1 WHERE urlname=\'"
						+ collection_id + "\');";
		qr.command = com2;
		qr2.priority = 9;
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
		query_str qr;
		string com =
				"UPDATE Torrents SET status=4 WHERE infohash IN (select C.infohash FROM Collections2Torrents C where C.urlname=\'"
						+ collection_id + "\');";
		qr.command = com;
		qr.priority = 9;
		db->addquery(&qr);

		query_str qr2;
		string com2 =
				"UPDATE Collections SET mirrored=0 WHERE urlname=\'"
						+ collection_id + "\');";
		qr.command = com2;
		qr2.priority = 9;
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
		std::string torrent_hash;
		std::vector < string > splits;
		std::string torrent_name;
		boost::split(splits, request_path, boost::is_any_of("/"));
		if (splits[splits.size() - 1] != "") {
			torrent_hash = splits[splits.size() - 1];
		} else {
			torrent_hash = splits[splits.size() - 2];
		}

		Database *db1 = new Database();
		db1->open(DATABASE_NAME);
		vector < vector<string> > results = db1->query(
				"Select T.filename FROM Torrents T WHERE T.infohash="
						+ torrent_hash + ";");
		db1->close();

		for (int i = 0; i < results.size(); i++) {
			vector < string > inner = results[i];
			for (int j = 0; j < inner.size(); j++) {
				torrent_name = inner[j];
			}
		}

		rep.status = reply::ok;
		rep.content.append(torrent_name);
		rep.headers.resize(2);
		rep.headers[0].name = "Content-Length";
		rep.headers[0].value = boost::lexical_cast < std::string
				> (rep.content.size());
		rep.headers[1].name = "Content-Type";
		rep.headers[1].value = mime_types::extension_to_type("json");

		return;
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
