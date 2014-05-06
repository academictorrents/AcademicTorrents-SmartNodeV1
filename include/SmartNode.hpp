/*
 * node_settings.hpp
 *
 *  Created on: Apr 28, 2014
 *      Author: nogueira
 */

#ifndef SmartNode_HPP_
#define SmartNode_HPP_

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <boost/asio/impl/src.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/function.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <libtorrent/entry.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/config.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/error_code.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/settings.hpp>
#include "async_at_connection.hpp"
#include "CSVReader.hpp"
#include "database.hpp"
#include "at_server.hpp"
#include "at_fetcher.hpp"


class smartnode{

private:
	struct node_settings{
		long max_size;
		int max_threads;
		std::string database_dir;
		std::string data_dir;
	};

	node_settings settings;
	libtorrent::session s;
	libtorrent::error_code ec;	
	
	boost::thread apiThread;
	boost::thread updateDataThread;
	boost::thread sessionThread;

	static void createServer();
	static void initDatabase(string);
	static void runSession(libtorrent::session *);



public:
	smartnode();
	void init();
	void loadSettings();
	void saveSettings();
	static bool query_new(vector<vector<string> >*)
	static bool query_active(vector<vector<string> >*);
	//void update_status(int, string);
	static void add_dl_to_ses(libtorrent::session*, vector<vector<string> >);
	static void add_sd_to_ses(libtorrent::session*, vector<vector<string> >);
	void shutdown();
};




#endif /* SmartNode_HPP_ */
