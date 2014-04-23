/*
 * at_fetcher.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: nogueira
 */
#include "at_fetcher.hpp"

	/*saves the infohash for when the callback is made.*/
	static string cur_infohash;

	static void insert_csv_info(boost::asio::streambuf *stream){
		std::ostringstream ss;
		ss << stream;
		std::string content = ss.str();
		CSVReader reader(content, DATABASE_NAME);
		reader.readAll();

		/* find Torrent that have yet to be downloaded*/

		Database *db = new Database(DATABASE_NAME);
		vector<vector<string> > results = db->query("SELECT infohash from Torrents WHERE torrentpath=\"\";");

		for(std::vector<vector<string> >::iterator it_outer = results.begin(); it_outer != results.end(); ++it_outer){
			vector<string> vec = *it_outer;
			for(std::vector<string>::iterator it_inner = vec.begin(); it_inner != vec.end(); ++it_inner){
				/*download the torrent file*/
				at_fetcher at_fetcher;
				at_fetcher.download_torrent_file(*it_inner);
			}
		} 

	}

	static void write_torrent_file(boost::asio::streambuf *stream){
		string filepath = "./collections/" + cur_infohash + ".torrent";
		std::string update_query;
		std::ostringstream ss;
		ofstream dot_torrent;
		//TODO create new folders for the collection and subfolders if needed when downloading torrent files.
		/*write response to file*/
		ss << stream;
		dot_torrent.open(filepath.c_str());
		dot_torrent << ss.str() << endl;
		dot_torrent.flush();
		dot_torrent.close();

		//TODO currently only check if the database has not path. Also check for the file in the directory in case of deletion.
		/*Update database with new file path.*/
		Database *db = new Database(DATABASE_NAME);
		update_query = "UPDATE Torrents SET torrentpath=\"" + filepath + "\" WHERE infohash=\""+ cur_infohash + "\";";
		db->query(update_query.c_str());
		db->close();

	}

	at_fetcher::at_fetcher(){

	}

	std::string at_fetcher::get_torrent_file_path(const std::string infohash){

		return "";
	}

	void at_fetcher::parse_collection_csv(const std::string collection){
		boost::asio::io_service io_service;
		async_at_client::async_at_connection c(io_service, AT_URL, at_fetcher::create_csv_url(collection), insert_csv_info);
		io_service.run();
	}

	std::string at_fetcher::get_collection_csv(const std::string collection){
		std::string collection_csv;

		return collection_csv;

	}

	void at_fetcher::download_torrent_file(const std::string infohash){
		cur_infohash = infohash;
		boost::asio::io_service io_service;
		async_at_client::async_at_connection c(io_service, AT_URL, at_fetcher::create_dot_torrent_url(cur_infohash), write_torrent_file);
		io_service.run();
	}

	/* Create csv url given collection name */
	std::string at_fetcher::create_csv_url(const std::string collection_name){
		/* ex: /collection/umass-boston-cs-department.csv */
		return "/collection/" + collection_name + ".csv";
	}

	std::string at_fetcher::create_dot_torrent_url(const std::string infohash){
		/* ex. /download/cb1655a57dd24345c9ea7a43c5ec09e03c7a0979.torrent */
		return "/download/" + infohash + ".torrent";
	}

	//TODO REMOVE TEST CODE
//	int main (){
//		at_fetcher fetcher;
//		fetcher.parse_collection_csv("umass-boston-cs-department");
////		fetcher.download_torrent_file("cb1655a57dd24345c9ea7a43c5ec09e03c7a0979");
//		return 0;
//	}

