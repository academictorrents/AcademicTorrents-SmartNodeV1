/*
 * at_fetcher.hpp
 *
 *  Created on: Mar 20, 2014
 *      Author: nogueira
 */

#ifndef AT_FETCHER_HPP_
#define AT_FETCHER_HPP_

#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iostream>
#include "async_at_connection.hpp"
#include "CSVReader.hpp"

class at_fetcher{

	public:
		at_fetcher();
		/* returns path to the .torrent file given infohash*/
		std::string get_torrent_file_path(const std::string infohash);
		/* pass name of collection*/
		void parse_collection_csv(const std::string collection);
		/* param infohash */
		void parse_collection_list(const std::string url);
		//param infohash
		void download_torrent_file(const std::string infohash);
	private:
		std::string get_collection_csv(const std::string collection);
		std::string create_torrent_file_url(const std::string infohash);
		std::string create_dot_torrent_url(const std::string infohash);
		std::string create_csv_url(const std::string collection_name);

};


#endif /* AT_FETCHER_HPP_ */
