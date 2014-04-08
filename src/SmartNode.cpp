/*
 * SmartNode.cpp
 *
 *  Created on: Mar 24, 2014
 *      Author: adrian
 */

#define BOOST_ASIO_SEPARATE_COMPILATION
//#define  BOOST_ASIO_DYN_LINK
#include <iostream>
//#include <string>
#include <vector>
#include <boost/asio/impl/src.hpp>
#include <boost/function.hpp>
#include <libtorrent/entry.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/config.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/error_code.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/torrent_info.hpp>
#include "async_at_connection.hpp"
#include "CSVReader.hpp"
#include "database.hpp"

#define DEBUG

#ifdef DEBUG



bool yes(libtorrent::torrent_status const&)
{ return true; }


int main(int argc, char **argv) {
	using namespace libtorrent;

	//Database *db = new Database(DATABASE_NAME);
	//vector<vector<string> > results = db->query("SELECT infohash from Torrents WHERE torrentpath!=\"\";");

	session s;
	error_code ec;
	s.listen_on(std::make_pair(6881, 6689), ec);
	if (ec) {
		fprintf(stderr, "failed to open listen socket: %s\n",
				ec.message().c_str());
		return 1;
	}
	add_torrent_params p;
	p.save_path = "./collections";
	std::cout << "saved path" << std::endl;
	p.ti = new torrent_info(argv[1], ec);
	p.auto_managed = true;
	std::cout << "added torrent info" << std::endl;
	if (ec) {
		fprintf(stderr, "%s\n", ec.message().c_str());
		return 1;
	}
	s.add_torrent(p, ec);
	//s.pause();
	std::cout << "added torrent to session" << std::endl;

	std::vector<torrent_status> ret;

	s.get_torrent_status(&ret, &yes, 0);

	if(ret.empty()){
		std::cout << "is empty" << std::endl;
	}else {
		std::cout << "not empty" << std::endl;
	}



	std::cout << "got here" << std::endl;
	//s.resume();
	while (ret[0].progress < 0.999) {
		std::cout << "not done" << std::endl;
		s.refresh_torrent_status(&ret);
		std::cout << ret[0].progress * 100 << std::endl;
	}

	if(ret[0].finished){
		std::cout << "done" << std::endl;
	}
	return 0;
}

#endif
//
//#ifndef DEBUG
//
//int main(int argc, char **argv) {
//
//	using namespace libtorrent;
//	//using namespace std;
//
//	bool running = true;
//	bool ses_running = false;
//	bool invalid = true;
//	std::string command = "";
//	std::string path = "/home/adrian/Desktop/torrent/";
//	char response = ' ';
//	libtorrent::add_torrent_params tparams;
//	int torCount = 0;
//
//	libtorrent::session ses;
//	libtorrent::error_code ec;
//	std::vector<torrent_handle> torrents;
//
//	std::cout << "Would you like to start a session? (Y/N)" << std::endl;
//	while (invalid) {
//		std::cin >> response;
//		if (response == 'Y' || response == 'y') {
//			std::cout << "Starting a new session" << std::endl;
//			ses.listen_on(std::make_pair(6881, 6889), ec);
//			if (ec) {
//				std::cerr << "failed to open listen socket: "
//				<< ec.message().c_str();
//				std::cout << std::endl;
//			} else {
//				ses_running = true;
//			}
//			std::cout
//			<< "The current path is where the down loaded content is saved is: "
//			<< path << std::endl;
//
//			while (invalid) {
//				std::cout << "Would you like to change the path? (Y/N)" << std::endl;
//				std::cin >> response;
//				std::cout << std::endl;
//				if (response == 'Y' || response == 'y') {
//					invalid = false;
//					std::cout << "What is the new path? ";
//					std::cin >> path;
//					std::cout << std::endl;
//					std::cout << "The new path has been set to: " << path << std::endl;
//				} else if (response == 'N' || response == 'n') {
//					invalid = false;
//				} else {
//					std::cout << "Invalid response! Please enter Y or N." << std::endl;
//				}
//			}
//		} else if (response == 'N' || response == 'n') {
//			invalid = false;
//			running = false;
//			std::cout << "Exiting... " << std::endl;
//			//return 0;
//		} else {
//			std::cout << "Invalid response! Please enter Y or N." << std::endl;
//		}
//	}
//
//	while (running) {
//		std::cin >> command;
//		std::cout << std::endl;
//		if (command == "add") {
//			if (ses_running) {
//				bool more = true;
//				std::string tPath = "";
//				while (more) {
//					std::cout << "What is the path of the torrent? ";
//					std::cin >> tPath;
//					std::cout << tPath << std::endl;
//					tparams.save_path = tPath;
//					std::cout << "got here" << std::endl;
//					tparams.ti = new libtorrent::torrent_info(tPath, ec);
//					if (ec) {
//						std::cerr << ec.message().c_str();
//						std::cout << std::endl;
//					}
//					ses.add_torrent(tparams, ec);
//					if (ec) {
//						std::cerr << ec.message().c_str();
//						std::cout << std::endl;
//					} else {
//						torCount++;
//						std::cout << "Torrent was successfully added." << std::endl;
//						torrents = ses.get_torrents();
//						while(!torrents[torCount-1].is_finished()) {
//							std::cout << "downloading" << std::endl;
//						}
//						std::cout << "done" << std::endl;
//					}
//
//					while (invalid) {
//						std::cout << "Would you like to add another torrent? (Y/N)"
//						<< std::endl;
//						std::cin >> response;
//						std::cout << std::endl;
//						if (response == 'Y' || response == 'y') {
//							invalid = false;
//						} else if (response == 'N' || response == 'n') {
//							invalid = false;
//							more = false;
//						} else {
//							std::cout << "Invalid response! Please enter Y or N." << std::endl;
//						}
//					}
//				}
//			} else {
//				std::cout << "A session is needed!" << std::endl;
//			}
//		}
//		else if (command == "exit" || command == "Exit") {
//			running = false;
//		}
//
//		else {
//
//		}
//
//	}
//	return 0;
//}
//
//#endif
