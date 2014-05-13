#include "database.hpp"

//Database *db;

/*bool comparer(const query_str lhs, const query_str rhs) const{
 return lhs.priority < rhs.priority;
 }
 */
Database::Database() {

	bool create = false;

	if (!boost::filesystem::exists(std::string("./").append(DATABASE_NAME))) {
		sqlite3_open(DATABASE_NAME, &database);
		create = true;
	}

	//       if (sqlite3_open_v2(DATABASE_NAME, &database, SQLITE_OPEN_NOMUTEX, NULL) != SQLITE_OK){
	if (sqlite3_open(DATABASE_NAME, &database) != SQLITE_OK) {
		cout << "database open error";
	}

	if (create) {
		cout << "Creating Database" << endl;
		query(TORRENTS_TABLE);
		query(COLLECTIONS_TABLE);
		query(COLLECTION2TORRENTS_TABLE);
		//query("PRAGMA journal_mode=WAL;");
		cout << "Created Database" << endl;
	}
	sqlite3_close(database);
	//MyPriQue query_Queue = new priority_queue(value_compare const & = comparer());
}

Database::~Database() {
}

Database *db;

bool Database::open(char* filename) {
	//, SQLITE_OPEN_FULLMUTEX, NULL

	if (sqlite3_open(filename, &database) == SQLITE_OK)
		return true;
	return false;

}
void Database::addquery(query_str qry) {
	std::cout << "adding query " << qry.command << std::endl;
	query_Queue.push(qry);
	cout << "query added" << std::endl;
}

void Database::addquery(query_str *qry) {
	std::cout << "adding query " << qry->command << std::endl;
	query_Queue.push(*qry);
	cout << "query added" << std::endl;
}

vector<vector<string> > Database::query(const std::string query) {
	sqlite3_config (SQLITE_CONFIG_MULTITHREAD);
	sqlite3_enable_shared_cache(1);
	vector < vector<string> > results;
	if (sqlite3_prepare_v2(database, query.c_str(), -1, &statement, 0)
			== SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		int result = 0;

//	        if(){
		for (int i = 0; i < cols; i++) {
			columns.push_back(std::string(sqlite3_column_name(statement, i)));
		}
//	        }

		while (true) {
			result = sqlite3_step(statement);

			if (result == SQLITE_ROW) {
				vector < string > values;
				for (int col = 0; col < cols; col++) {
					std::string val;
					char * ptr = (char*) sqlite3_column_text(statement, col);
					if (ptr) {
						val = ptr;
					} else
						val = ""; // this can be commented out since std::string  val;
								  // initialize variable 'val' to empty string anyway
					values.push_back(val);  // now we will never push NULL
				}
				results.push_back(values);
			} else {
				break;
			}

		}
		sqlite3_finalize(statement);
	}
	string error = sqlite3_errmsg(database);
	if (error != "not an error")
		cout << query << " " << error << endl;
	return results;
}

void Database::makequery() {
	while (true) {
		while (!query_Queue.empty()) {
			int s = query_Queue.size();
			cout << "size: " << s << endl;
//			query_str current =  query_Queue.top();
			std::string query_String(query_Queue.top().command);
			std::cout << "making query " << query_String << std::endl;
			db->open(DATABASE_NAME);
			vector < vector<string> > res = query(query_String);
			db->close();
			query_Queue.pop();
			std::cout << "done with query" << std::endl;
		}
	}
}

bool Database::queue_empty(){
	return query_Queue.empty();
}
/*
 * returns the columns from the last select statement
 */
vector<string> Database::getColNames() {
//    sqlite3_stmt *statement;
//	int column_num = sqlite3_column_count(statement);
//	for(int i = 0; i < column_num; i++){
//		columns[i] = sqlite3_column_name(statement, i);
//	}
	return columns;
}

void Database::close() {
	sqlite3_close(database);
}

void Database::updateCollectionsList() {
	//create an Academic Torrent fetcher
	at_fetcher fetcher;
	//TODO create cleaner end point
	fetcher.parse_collection_list("/collections.php?format=.csv");
}

//gets torrent information give collection
void Database::updateTorrents(string collection_urlname) {
	at_fetcher fetcher;
	fetcher.parse_collection_csv(collection_urlname);
}

void Database::updateTableInfo() {

	updateCollectionsList();
	sleep(10);
	
	Database *db1 = new Database();
	cout << "aaaahhhhh!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
	db1->open(DATABASE_NAME);
	vector < vector<string> > results;
	//vector < vector<string> > results_temp = db1->query("select urlname from collections;");
	do {
		//sleep(1000);
		//cout << "running!!!!!!!!!!!" << endl;		
		//results = results_temp;
		//results_temp = db1->query("select urlname from collections;");
		results = db1->query("select urlname from collections;");
	}
	while (!db->queue_empty());
	//while (results.empty() || results_temp.size() != results.size());

	db1->close();
	cout << results.size();
	/*vector < vector<string> > results;
	 qr.result = &results;
	 qr.returned = false;
	 db->addquery(qr);
	 cout << "waiting for query";
	 cout << qr.returned;
	 cout << results.size();*/
	for (int i = 0; i < results.size(); i++) {
		vector < string > inner = results[i];
		for (int j = 0; j < inner.size(); j++) {
			cout << inner[j];
			updateTorrents (inner[j]);
		}
	}
}

query_str::query_str() {

}

int query_str::operator<(const query_str other) const {
	return this->priority < other.priority;
}

int query_str::operator>(const query_str other) const {
	return this->priority > other.priority;
}

//void query_str::swap(query_str& obj){
//	std::swap(this->command, obj.command);
//	std::swap(this->priority, obj.priority);
//}

query_str& query_str::operator=(query_str& a) {
//	a.swap(*this);
	std::swap(command, a.command);
	std::swap(priority, a.priority);
	return *this;
}



