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
//Database *db;
using namespace libtorrent;

smartnode::smartnode() {
	loadSettings();
	saveSettings();
}

bool yes(libtorrent::torrent_status const&) {
	return true;
}

static void folderCreator(string path) {
	namespace fs = boost::filesystem;
	if (!(fs::exists(path.c_str()))) {
		string comand = "mkdir " + path;
		system(comand.c_str());
	}
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

void smartnode::runningquery() {
	db->makequery();
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
		smartnode::settings.data_dir = json_settings.get<std::string>(
				"settings.data_dir");
		//smartnode::settings.sub_collection = json_settings.get<vector<string> >(
		//		"settings.sub_collection");
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
	arr.push_back(
		std::make_pair("data_dir",
				boost::lexical_cast < std::string
						> (settings.data_dir)));
	arr.push_back(
		std::make_pair("current_size",
				boost::lexical_cast < std::string
						> (settings.current_size)));
	//arr.push_back(
	//	std::make_pair("sub_collection",
	//			boost::lexical_cast < std::vector<string> 
	//					> (settings.sub_collection)));

	json_settings.put_child("settings", arr);

	ofstream output;
	output.open("./smart_node_settings.json");
	stringstream os;
	write_json(os, json_settings);
	output << os.str();
	output.close();
}

void smartnode::initDatabase(string database_location) {

		//update collections in database on startup
		Database *db_update = new Database();	
		db_update->updateTableInfo();
		sleep(3800);
}

bool smartnode::check4file(string path, int size) {
	namespace fs = boost::filesystem;
	if (fs::exists(path.c_str())) {
		if (file_size(path.c_str()) == size) {
			return true;
		} else {
			cout << "part of the " << path << " is missing!! " << endl;
			return false;
		}
		cout << "creating " << path << endl;
		return false;
	}
	return false;
}

void update_status(int status, string infohash) {

	string update_query = "UPDATE Torrents SET status=" + boost::lexical_cast
			< string > (status) + " WHERE infohash=\"" + infohash + "\";";

	query_str qrs;
	qrs.command = update_query;
	qrs.priority = 8;
	db->addquery(&qrs);
} 

void update_filename(std::string filename, std::string infohash){
/*
	fs::path someDir("./data/"+ infohash + filename);
	fs::directory_iterator end_iter;

	if ( fs::exists(someDir) && fs::is_directory(someDir)){
		    for( fs::directory_iterator dir_iter(someDir) ; dir_iter != end_iter ; ++dir_iter){
			if (fs::is_regular_file(dir_iter->status()) ){
				filename = "";			      
			  }
		      }
		} 
*/
		string update_query = "UPDATE Torrents SET filename=\"" + filename + "\" WHERE infohash=\"" + infohash + "\";";
		query_str qrs;
		qrs.command = update_query;
		qrs.priority = 8;
		db->addquery(&qrs);  
}

void smartnode::add_to_ses(int flag, libtorrent::session* ses) {
	
        Database *db1 = new Database();
	db1->open(DATABASE_NAME);
	vector<vector<string> > torrs = db1->query(
			"SELECT DISTINCT torrentpath, infohash, sizebytes, name from Torrents WHERE status = "
					+ boost::lexical_cast < string > (flag) + ";");
	
	db1->close();
	if(!torrs.empty()){
	  cout << "adding to session!!!"<< std::endl;
	  for (vector<vector<string> >::iterator it_outer = torrs.begin();
	       it_outer != torrs.end(); ++it_outer) {
	    vector < string > vec = *it_outer;
	    libtorrent::add_torrent_params *p = new libtorrent::add_torrent_params();
	    std::string infohash = vec[1];
		if(settings.max_size < current_size + atol((vec[2]).c_str())){
	    	cout << "Torrent added" << infohash << endl;

	    	string path = settings.data_dir + infohash + "/";
	    	if (flag == WANT) {
	      		//making folder
	      		folderCreator(path);
	      		update_status(DOWNLOAD, infohash);
	    	}

			current_size += atol((vec[2]).c_str()); 
			cout << "bytes: "<< current_size << endl;
		

	    	p->save_path = path;
	    	p->ti = new libtorrent::torrent_info(vec[0]);
	    	ses->add_torrent(*p);
	  	
		}else{
			cout << "can not add torrent: " << vec[3] << endl; 
		}
	}
	}
}

void smartnode::runSession() {
       
	current_size = 0;
	cout << "bytes: "<< current_size << endl;
        //add torrents that been downloaded or are in the process
	add_to_ses(SEED, &s);
	add_to_ses(DOWNLOAD, &s);

	session s;
	libtorrent::error_code ec;

	session_settings settings = s.settings();
	settings.version = 1;
	settings.user_agent = "SmartNode";
	s.set_settings(settings);

	sleep(30);
	cout
			<< "Starting session now"
			<< endl;
	s.listen_on(std::make_pair(6800, 6800), ec);
	add_to_ses(DOWNLOAD, &s);
	
	bool new_torrs;
	int count = 0;
	while (1) {
	  
		//adding new torrents that need to be downloaded
		add_to_ses(WANT, &s);
		
		std::vector<torrent_handle> torrents_in_session = s.get_torrents();
		torrent_handle t_handler;
	    
		cout << torrents_in_session.size();
		
		for(int j = 0; j < torrents_in_session.size(); j++){
		  t_handler = torrents_in_session[j]; 
		  std::stringstream ss_sha; 
		  ss_sha << t_handler.info_hash();
		  
		  bool seeding = t_handler.is_seed();
		  if(seeding){	
		    seeds.insert(ss_sha.str());
		    update_filename(t_handler.name(), ss_sha.str());
		    update_status(SEED, ss_sha.str());
		  }	
		}
		/*std::vector < torrent_status > ret;
		s.get_torrent_status(&ret, &yes, 0);
		torrent_status ts;
		for (int t = 0; t < ret.size(); t++) {
			ts = ret[t];
			while (!ts.finished) {
				s.refresh_torrent_status(&ret);
				ts = ret[t];
				std::cout << ts.progress * 100 << std::endl;
			}
			//break shit.
			string tor_info_hash(ts.info_hash.to_string());
			update_status(SEED, tor_info_hash);
			}
		*/
		//2 mins.
	       sleep(120);
	}
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
	queryThread = boost::thread(&runningquery);
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
	if (!fs::exists("./smartnode_log.log")) {
		std::ofstream outfile("./smartnode_log.log");
		outfile.close();
	}

	filestr.open("./smartnode_log.log");

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

	node.runSession();

	std::cout.rdbuf(backup);
	filestr.close();

	
}

