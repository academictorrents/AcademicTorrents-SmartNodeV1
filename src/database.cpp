#include "database.hpp"

Database::Database()
{
    database = NULL;
}

Database::~Database()
{}

bool Database::open(char* filename)
{
    if(sqlite3_open(filename, &database) == SQLITE_OK)
        return true;
    return false;
}
vector<vector<string> > Database::query(const std::string query)
{

	    vector<vector<string> > results;
	    if(sqlite3_prepare_v2(database, query.c_str(), -1, &statement, 0) == SQLITE_OK)
	    {
	        int cols = sqlite3_column_count(statement);
	        int result = 0;

//	        if(){
				for(int i = 0; i < cols; i++){
						columns.push_back(std::string(sqlite3_column_name(statement, i)));
					}
//	        }

	        while(true)
	        {
	            result = sqlite3_step(statement);

	            if(result == SQLITE_ROW)
	            {
	               vector<string> values;
	               for(int col = 0; col < cols; col++)
	               {
	                 std::string  val;
	                 char * ptr = (char*)sqlite3_column_text(statement, col);
	                 if(ptr)
	                 {
	                   val = ptr;
	                 }
	                 else val = ""; // this can be commented out since std::string  val;
	                                // initialize variable 'val' to empty string anyway
	                 values.push_back(val);  // now we will never push NULL
	               }
	               results.push_back(values);
	            }
	            else
	            {
	               break;
	            }

	        }

//	    	for(int i = 0; i < cols; i++){
//	    		columns[i] = sqlite3_column_name(statement, 1);
//	    	}

//	        cout << sqlite3_column_name(statement, 1);

	        sqlite3_finalize(statement);
	    }

	    string error = sqlite3_errmsg(database);
	    if(error != "not an error") cout << query << " " << error << endl;
	    return results;
}
/*
 * returns the columns from the last select statement
 */

vector<string> Database::getColNames(){
//    sqlite3_stmt *statement;
//	int column_num = sqlite3_column_count(statement);
//	for(int i = 0; i < column_num; i++){
//		columns[i] = sqlite3_column_name(statement, i);
//	}
	return columns;
}

void Database::close()
{
    sqlite3_close(database);
}

void Database::updateCollectionsList(){
	//create an Academic Torrent fetcher
	at_fetcher fetcher;
	//TODO create cleaner end point
	fetcher.parse_collection_list("/collections.php?format=.csv");
}

//gets torrent information give collection
void Database::updateTorrents(string collection_urlname){
	at_fetcher fetcher;
	fetcher.parse_collection_csv(collection_urlname);
}

void Database::updateTableInfo(){

	updateCollectionsList();

	Database *db = new Database();
	db->open(DATABASE_NAME);
	vector<vector<string> > results = db->query("select urlname from collections;");

	for(int i = 0; i < results.size(); i++){
		vector<string> inner = results[i];
		for(int j = 0; j < inner.size(); j++){
			updateTorrents(inner[j]);
		}
	}
}
