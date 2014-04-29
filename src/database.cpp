#include "database.h"
#include <iostream>
database::database()
{
	data_base = NULL;
    open(DATABASE_NAME);
    query(TORRENTS_TABLE);
    query(COLLECTIONS_TABLE);
    query(COLLECTIONS2TORRENTS_TABLE);
}

database::~database()
{}

bool database::open(char* filename)
{
    if(sqlite3_open(filename, &data_base) == SQLITE_OK)
        return true;
    return false;
}
vector<vector<string> > database::query(char* query)
{
    sqlite3_stmt *statement;
    vector<vector<string> > results;
    if(sqlite3_prepare_v2(data_base, query, -1, &statement, 0) == SQLITE_OK)
    {
        int cols = sqlite3_column_count(statement);
        int result = 0;
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

        sqlite3_finalize(statement);
    }

    string error = sqlite3_errmsg(data_base);
    if(error != "not an error") cout << query << " " << error << endl;
    return results;
}
void database::close()
{
    sqlite3_close(data_base);
}
