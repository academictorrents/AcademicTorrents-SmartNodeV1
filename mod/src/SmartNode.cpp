///*

// * SmartNode.cpp
// *
// *  Created on: Mar 24, 2014
// *      Author: adrian
// */

#define BOOST_ASIO_SEPARATE_COMPILATION
//#define  BOOST_ASIO_DYN_LINK
#include "SmartNode.hpp"

//#define DEBUG

//#ifdef DEBUG

//extern Database *db;
Database *db;
using namespace libtorrent;

smartnode::smartnode() {
	loadSettings();
	saveSettings();
}

bool yes(libtorrent::torrent_status const&) {
	return true;
}

static void folderCreator(string path) {
	string comand = "mkdir " + path;
	system(comand.c_str()); 
}
void smartnode::createServer() {
	try {

		// Initialise the server.
		std::size_t num_threads = boost::lexical_cast < std::size_t > (3);
		http::at_server::server s("at01.cs.umb.edu", "6801", "./public_html",
				num_threads);

		// Run the server until stopped.
		s.run();
	} catch (std::exception& e) {
		std::cerr << "exception: " << e.what() << "\n";
	}
}

void smartnode::loadSettings() {
	ptree json_settings;
	std::stringstream json_input;
	ifstream file("./smart_node_settings.json");
	if (file.is_open()) {
		json_input << file.rdbuf();
		file.close();
		read_json(json_input, json_settings);

//	json_settings = json_settings.get_child("settings");

//load settings from file
		smartnode::settings.database_dir = json_settings.get < std::string
				> ("settings.database_dir");
		smartnode::settings.max_size = json_settings.get<long>(
				"settings.max_size");
		smartnode::settings.max_threads = json_settings.get<int>(
				"settings.max_threads");
	} else {
		//TODO make this create a new file or output an error
		cout << "error";
	}
}

void smartnode::saveSettings() {
	ptree json_settings, arr;
	std::stringstream json_input;

	arr.push_back(std::make_pair("database_dir", settings.database_dir));
	//convert int/long to string for json
	arr.push_back(
			std::make_pair("max_size",
					boost::lexical_cast < std::string > (settings.max_size)));
	arr.push_back(
			std::make_pair("max_threads",
					boost::lexical_cast < std::string
							> (settings.max_threads)));

	json_settings.put_child("settings", arr);

	ofstream output;
	output.open("./smart_node_settings.json");
	stringstream os;
	write_json(os, json_settings);
	output << os.str();
	output.close();
}

void smartnode::initDatabase(string database_location) {

	/*
	 * If the database file doesn't exist a new is created.
	 */

	//Database *db = new Database();

	//look for a database
	if (!boost::filesystem::exists(database_location.append(DATABASE_NAME))) {
		cout << "Creating Database";
		//db_database->open(DATABASE_NAME);

		//create tables
		/*		db_database->query(TORRENTS_TABLE);
		db_database->query(COLLECTIONS_TABLE);
		db_database->query(COLLECTION2TORRENTS_TABLE);
		db_database->close();
		cout << "Database Created!";
		*///done with the database connection
	}

	while(1){
	  //update collections in database on startup
	   db->updateTableInfo();
	  //sleep for 2 mins
	  sleep(60000);
	}

}

bool smartnode::check4file(string path, int size){
	namespace fs = boost::filesystem;
	if(fs::exists(path.c_str())){
		if(file_size(path.c_str()) == size){
			return true;
		}else{
			cout << "part of the " << path << " is missing!! " << endl;
			return false;
		}
		cout << "creating " << path << endl;
		return false;
	}
}

void update_status(int status, string infohash) {
	string update_query = "UPDATE Torrents SET status=" + boost::lexical_cast
			< string > (status) + " WHERE infohash=\"" + infohash + "\";";
//	Database *db = new Database();
	//db->open(DATABASE_NAME);
	db->query(update_query.c_str());
	db->close();
}



void smartnode::add_to_ses(int flag, libtorrent::session* ses,
		vector<vector<string> > tors) {
	for (vector<vector<string> >::iterator it_outer = tors.begin();
			it_outer != tors.end(); ++it_outer) {
		vector < string > vec = *it_outer;

		libtorrent::add_torrent_params p;
		cout << vec.at(1) << endl;

		string path = settings.data_dir + vec.at(1) + "/";
		if (flag == WANT  && !check4file(path, atoi(vec.at(2).c_str()))) {
			//making folder
			folderCreator(path);
			update_status(DOWNLOAD, vec.at(1));
		}else if((flag == SEED || flag == DOWNLOAD) && !check4file(path, atoi(vec.at(2).c_str()))){
			cout << "error!! " << path << "may have been remove" << endl;
		}

		p.save_path = path;
		p.ti = new libtorrent::torrent_info(vec.at(0));
		ses->add_torrent(p);
	}
}


bool smartnode::query_status(int status, vector<vector<string> >* tor) {
//	Database *db = new Database();
	//db->open(DATABASE_NAME);
	*tor = db->query(
			"SELECT torrentpath, infohash from Torrents WHERE status = "
					+ boost::lexical_cast < string > (status) + ";");
	db->close();
	return !tor->empty();
}

void smartnode::runSession() {
	libtorrent::error_code ec;
	s.listen_on(std::make_pair(6800, 6800), ec);

	vector < vector<string> > tors;
	//vector < vector<string> > seeds;
	// add the torrents the just need to see
	if (query_status(SEED, &tors)) {
		add_to_ses(SEED, &s, tors);
	}
	if (query_status(DOWNLOAD, &tors)) {
			add_to_ses(DOWNLOAD, &s, tors);
	}
	while (1) {

		//adding new torrents that need to be downloaded
		if (query_status(WANT, &tors)) {
			add_to_ses(DOWNLOAD, &s, tors);
		}

		std::vector < torrent_status > ret;
		s.get_torrent_status(&ret, &yes, 0);
		torrent_status ts;
		for (int t = 0; t < ret.size(); t++) {
			ts = ret[t];
			while (!ts.finished) {
				s.refresh_torrent_status(&ret);
				ts = ret[t];
				std::cout << ts.progress * 100 << std::endl;
			}
			update_status(SEED, ts.info_hash.to_string());
		}
	}

	/*
	 while (true)
	 ;
	 */

}

//smart node start up
void smartnode::init() {
	smartnode::loadSettings();
	db = new Database();
		//	initDatabase(settings.database_dir);
	//TODO make this thread a timed task solve database locking
	updateDataThread = boost::thread(&initDatabase, settings.database_dir);
	//start API on shutdown need to stop thread
	apiThread = boost::thread(&createServer);
	//sessionThread = boost::thread(&runSession, &s);
}

void smartnode::shutdown() {

	//wait for API thread to exit.
	//TODO interrupt thread to exit. currently hangs.
	apiThread.join();
}

int main(int argc, char **argv) {
	using namespace libtorrent;

	std::streambuf *psbuf, *backup;
	std::ofstream filestr;

	namespace fs = boost::filesystem;
	if(!fs::exists("./smartnode_log.log")){
		std::ofstream outfile ("./smartnode_log.log");
		outfile.close();
	}

	filestr.open ("./smartnode_log.log");
	
	backup = std::cout.rdbuf();
	
	psbuf = filestr.rdbuf();
	std::cout.rdbuf(psbuf);
	
	pid_t pid, sid;

	pid = fork();

	if (pid < 0) {
		exit (EXIT_FAILURE);
	}
	if (pid > 0) {
		exit (EXIT_SUCCESS);
	}

	/* Change the file mode mask */
	umask(0);

	sid = setsid();
	if (sid < 0) {
		/* Log the failure */
		exit (EXIT_FAILURE);
	}

	smartnode node;
	node.init();

	session s;

	session_settings settings = s.settings();
	settings.version = 1;
	settings.user_agent = "SmartNode";
	
	sleep(180000);
	cout << "wake up!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
	node.runSession();
	
	std::cout.rdbuf(backup);
	filestr.close();
	
	while(1){

	}
}

