///*
// * SmartNode.cpp
// *
// *  Created on: Mar 24, 2014
// *      Author: adrian
// */

#define BOOST_ASIO_SEPARATE_COMPILATION
//#define  BOOST_ASIO_DYN_LINK
#include "SmartNode.hpp"

#define DEBUG

#ifdef DEBUG

using namespace libtorrent;

smartnode::smartnode(){
	loadSettings();
	saveSettings();
}

bool yes(libtorrent::torrent_status const&)
{ return true; }

static void folderCreator(string path) {
	string comand = "mkdir " + path;
	system(comand.c_str());
}
void smartnode::createServer(){
	  try
	  {

	    // Initialise the server.
	    std::size_t num_threads = boost::lexical_cast<std::size_t>(3);
	    http::at_server::server s("at01.cs.umb.edu", "6801", "./public_html", num_threads);

	    // Run the server until stopped.
	    s.run();
	  }
	  catch (std::exception& e)
	  {
	    std::cerr << "exception: " << e.what() << "\n";
	  }
}

void smartnode::loadSettings(){
	ptree json_settings;
	std::stringstream json_input;
	ifstream file("./smart_node_settings.json");
	if(file.is_open()){
		json_input << file.rdbuf();
		file.close();
		read_json(json_input, json_settings);

//	json_settings = json_settings.get_child("settings");

	//load settings from file
	smartnode::settings.database_dir = json_settings.get<std::string>("settings.database_dir");
	smartnode::settings.max_size = json_settings.get<long>("settings.max_size");
	smartnode::settings.max_threads = json_settings.get<int>("settings.max_threads");
	} else {
		//TODO make this create a new file or output an error
		cout << "error";
	}
}

void smartnode::saveSettings(){
	ptree json_settings, arr;
	std::stringstream json_input;


	arr.push_back(std::make_pair("database_dir", settings.database_dir));
	//convert int/long to string for json
	arr.push_back(std::make_pair("max_size", boost::lexical_cast<std::string>(settings.max_size)));
	arr.push_back(std::make_pair("max_threads", boost::lexical_cast<std::string>(settings.max_threads)));

	json_settings.put_child("settings",arr);

	ofstream output;
	output.open("./smart_node_settings.json");
	stringstream os;
	write_json(os, json_settings);
	output << os.str();
	output.close();
}

void smartnode::initDatabase(string database_location){

	/*
	 * If the database file doesn't exist a new is created.
	 */

	Database *db = new Database();

	//look for a database
	if(!boost::filesystem::exists(database_location.append(DATABASE_NAME))){
		cout << "Creating Database";
		db->open(DATABASE_NAME);

		//create tables
		db->query(TORRENTS_TABLE);
		db->query(COLLECTIONS_TABLE); 
		db->query(COLLECTION2TORRENTS_TABLE);
		db->close();
		db->updateTableInfo();
		cout << "Database Created!";
		//done with the database connection
	} else {
		//update collections in database on startup
//		db->updateTableInfo();
	}
}

void update_status(int status, string infohash){
	string update_query = "UPDATE Torrents SET status=" + boost::lexical_cast<string> (status) + " WHERE infohash=\""+ infohash + "\";";
	Database *db = new Database();
	db->query(update_query.c_str());
	db->close();
}

void smartnode::add_dl_to_ses(libtorrent::session* ses, vector<vector<string> > tors){
	for (vector<vector<string> >::iterator it_outer = tors.begin();
				it_outer != tors.end(); ++it_outer) {
			vector<string> vec = *it_outer;

			libtorrent::add_torrent_params p;
			cout << vec.at(1) << endl;

			//cout << "got here" << endl;
			//making folder
			string path = settings.data_dir + vec.at(1) + "/";
			folderCreator(path);
			p.save_path = path;
			p.ti = new libtorrent::torrent_info(vec.at(0));
			ses->add_torrent(p);
		}
}

void smartnode::add_sd_to_ses(libtorrent::session* ses, vector<vector<string> > tors){
	for (vector<vector<string> >::iterator it_outer = tors.begin();
				it_outer != tors.end(); ++it_outer) {
			vector<string> vec = *it_outer;

			libtorrent::add_torrent_params p;
			cout << vec.at(1) << endl;

			//making folder
			string path = settings.data_dir + vec.at(1) + "/";
			//folderCreator(path);
			p.save_path = path;
			p.ti = new libtorrent::torrent_info(vec.at(0));
			ses->add_torrent(p);
		}
}

bool smartnode::query_new(vector<vector<string> >* tor) {
	Database *db = new Database();
	*tor =
			db->query(
					"SELECT torrentpath, infohash from Torrents WHERE status = " + boost::lexical_cast<string>(DOWNLOAD) + ";");
	db->close();
	return !tor->empty();
}

bool smartnode::query_active(vector<vector<string> >* seed) {
	Database *db = new Database();
	//db->open(DATABASE_NAME);
	*seed =
			db->query(
					"SELECT torrentpath, infohash from Torrents WHERE status = " + boost::lexical_cast<string>(SEED) + ";");
	db->close();
	return !seed->empty();
}

void smartnode::runSession(libtorrent::session* ses){
	libtorrent::error_code ec;
	ses->listen_on(std::make_pair(6800, 6800), ec);
	// add the torrents the just need to seed
	vector<vector<string> > tors;
	vector<vector<string> > seeds;	
	if(query_active(&seeds)){
		add_sd_to_ses(ses, seeds);
	}
	//adding new torrents that need to be downloaded
	if (query_new(&tors)){
		add_dl_to_ses(ses, tors);
	}	
	
	std::vector<torrent_status> ret;
	ses->get_torrent_status(&ret, &yes, 0);
	torrent_status ts;
	for (int t = 0; t < ret.size(); t++) {
		ts = ret[t];
		while (!ts.finished) {
			ses->refresh_torrent_status(&ret);
			ts = ret[t];
			std::cout << ts.progress * 100 << std::endl;
		}
		update_status(SEED, ts.info_hash.to_string());
	}
	/*
	while (true)
		;
	*/
	
}



//smart node start up
void smartnode::init(){
	smartnode::loadSettings();

	//start API on shutdown need to stop thread
	apiThread = boost::thread(&createServer);
	//	initDatabase(settings.database_dir);
	//TODO make this thread a timed task solve database locking
	 updateDataThread = boost::thread(&initDatabase,settings.database_dir);

	sessionThread = boost::thread(&runSession, &s);
}

void smartnode::shutdown(){

	//wait for API thread to exit.
	//TODO interrupt thread to exit. currently hangs.
	apiThread.join();
}




int main(int argc, char **argv) {
	using namespace libtorrent;

	pid_t pid, sid;

	pid = fork();

	if(pid < 0){
		exit(EXIT_FAILURE);
	}
	if(pid > 0){
		exit(EXIT_SUCCESS);
	}

	/* Change the file mode mask */
	umask(0);

    sid = setsid();
    if (sid < 0) {
            /* Log the failure */
            exit(EXIT_FAILURE);
    }

	smartnode node;
	node.init();

	Database *db = new Database();
	db->open(DATABASE_NAME);
	vector<vector<string> > results = db->query("SELECT torrentpath from Torrents WHERE torrentpath!=\"\";");
	db->close();

	session s;

	session_settings settings = s.settings();
	settings.version = 1;
	settings.user_agent = "SmartNode";

	error_code ec;
	s.listen_on(std::make_pair(6800, 6800));
	if (ec) {
		fprintf(stderr, "failed to open listen socket: %s\n",
				ec.message().c_str());
		return 1;
	}


//	for(std::vector<vector<string> >::iterator it_outer = results.begin(); it_outer != results.end(); ++it_outer){
//	  vector<string> vec = *it_outer;
//	  for(std::vector<string>::iterator it_inner = vec.begin(); it_inner != vec.end(); ++it_inner){
//	    /*download the file*/
//	    std::cout << *it_inner << std::endl;
//	    add_torrent_params p;
//	    p.save_path = "./data/";
//	    std::cout << "saved path" << std::endl;
//	    p.ti = new torrent_info(*it_inner);
//	    p.auto_managed = true;
//	    std::cout << "added torrent info" << std::endl;
//	    if (ec) {
//	      fprintf(stderr, "%s\n", ec.message().c_str());
//	      return 1;
//	    }
//	    s.add_torrent(p);
//	    //s.pause();
//	    std::cout << "added torrent to session" << std::endl;
//	  }
//	}
//
//	std::vector<torrent_status> ret;

//	s.get_torrent_status(&ret, &yes, 0);
//
//	if(ret.empty()){
//		std::cout << "is empty" << std::endl;
//	}else {
//		std::cout << "not empty" << std::endl;
//	}



//	//std::cout << "got here" << std::endl;
//	//s.resume();
//	while (ret[0].progress < 0.999) {
//		std::cout << "not done" << std::endl;
//		s.refresh_torrent_status(&ret);
//		std::cout << ret[0].progress * 100 << std::endl;
//	}
//
//	if(ret[0].finished){
//		std::cout << "done" << std::endl;
//	}

	//wait for thread to finish
	while(1){
//	node.shutdown();
    }

//	return 0;
    exit(EXIT_SUCCESS);
}
  
#endif

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
