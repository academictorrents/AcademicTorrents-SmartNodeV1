/*
 * at_fetcher.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: nogueira
 */
#include "at_fetcher.hpp"

//extern Database *db;

/*saves the infohash for when the callback is made.*/
static string cur_infohash;

static void insert_collection_csv_info(boost::asio::streambuf *stream,
		string collection) {
	std::ostringstream ss;
	ss << stream;
	std::string content = ss.str();
	CSVReader reader(content, DATABASE_NAME);
	reader.readAll(collection);

	/* find Torrent that have yet to be downloaded*/

	Database *db1 = new Database();
	//sleep(100000);
	sleep(20);
	db1->open(DATABASE_NAME);
	
	
	vector < vector<string> > results;
	while (!db->queue_empty());
		cout << "im doing stuff!!!!!!!!!!" << endl;
		results = db1->query( "SELECT DISTINCT T.infohash from Torrents T JOIN Collections2Torrents CT ON T.infohash=CT.infohash WHERE CT.urlname=\"" + collection + "\" AND T.torrentpath=\"NULL\";");
	
	
	db1->close();

	for (std::vector<vector<string> >::iterator it_outer = results.begin();
			it_outer != results.end(); ++it_outer) {
		vector < string > vec = *it_outer;
		for (std::vector<string>::iterator it_inner = vec.begin();
				it_inner != vec.end(); ++it_inner) {
			/*download the torrent file*/
			at_fetcher at_fetcher;
			cout << "downloading!!!!!!!!!!!!" << endl;
			at_fetcher.download_torrent_file(*it_inner);
		}
	}
}

static void write_torrent_file(boost::asio::streambuf *stream, string) {
	string filepath = "./data/torrent_files/" + cur_infohash + ".torrent";
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

	//TODO currently only check if the database has no path. Also check for the file in the directory in case of deletion.
	/*Update database with new file path.*/
//		Database *db = new Database();
//	update_query = "UPDATE Torrents SET torrentpath=\"" + filepath
//			+ "\" WHERE infohash=\"" + cur_infohash + "\";";
	//db->open(DATABASE_NAME);
//	db->query(update_query.c_str());
	//db->close();
	query_str qr;
	qr.command = "UPDATE Torrents SET torrentpath=\"" + filepath + "\" WHERE infohash=\"" + cur_infohash + "\";";
	qr.priority = 4;
	db->addquery(&qr);
}

static void write_collections_list(boost::asio::streambuf *stream, string s) {
	std::ostringstream ss;
	ss << stream;
	std::string content = ss.str();
	CSVReader reader(content, DATABASE_NAME);
	reader.readCollections();
}

at_fetcher::at_fetcher() {

}

std::string at_fetcher::get_torrent_file_path(const std::string infohash) {

	return "";
}

void at_fetcher::parse_collection_csv(const std::string collection) {
	boost::asio::io_service io_service;
	async_at_client::async_at_connection c(io_service, AT_URL,
			at_fetcher::create_csv_url(collection), insert_collection_csv_info,
			collection);
	io_service.run();
}

std::string at_fetcher::get_collection_csv(const std::string collection) {
	std::string collection_csv;

	return collection_csv;

}

void at_fetcher::parse_collection_list(const std::string path) {
	boost::asio::io_service io_service;
	async_at_client::async_at_connection c(io_service, AT_URL, path, write_collections_list, "");
	io_service.run();
}

void at_fetcher::download_torrent_file(const std::string infohash) {
	cur_infohash = infohash;
	namespace fs = boost::filesystem;
	std::string path = "./data/torrent_files/" + infohash + ".torrent"; 
	if (!fs::exists(path.c_str())) {
		boost::asio::io_service io_service;
		async_at_client::async_at_connection c(io_service, AT_URL,
			at_fetcher::create_dot_torrent_url(cur_infohash),		
			write_torrent_file, "");
		io_service.run();
	}else {
	string filepath = "./data/torrent_files/" + cur_infohash + ".torrent";
	query_str qr;
	qr.command = "UPDATE Torrents SET torrentpath=\"" + filepath + "\" WHERE infohash=\"" + cur_infohash + "\";";
	qr.priority = 4;
	db->addquery(&qr);
	}	
}

/* Create csv url given collection name */
std::string at_fetcher::create_csv_url(const std::string collection_name) {
	/* ex: /collection/umass-boston-cs-department.csv */
	return "/collection/" + collection_name + ".csv";
}

std::string at_fetcher::create_dot_torrent_url(const std::string infohash) {
	/* ex. /download/cb1655a57dd24345c9ea7a43c5ec09e03c7a0979.torrent */
	return "/download/" + infohash + ".torrent";
}
