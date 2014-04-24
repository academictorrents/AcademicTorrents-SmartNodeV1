#ifndef __DATABASE_H__
#define __DATABASE_H__
#include <string>
#include <vector>
#include <sqlite3.h>

#define TORRENTS_TABLE "create table if not exists Torrents (type TEXT, name TEXT,infohash TEXT  PRIMARY KEY,sizebytes BIGINT,mirrors INTEGER,downloaders INTEGER,timescompleted INTEGER,dateadded TEXT,datemodified TEXT, torrentpath TEXT DEFAULT 'NULL');"
/*
#define DATABASE_TABLES "CREATE TABLE Torrents (type TEXT, name TEXT,infohash TEXT  PRIMARY KEY,sizebytes BIGINT,mirrors INTEGER,downloaders INTEGER,timescompleted INTEGER,
dateadded TEXT,datemodified TEXT, torrentpath TEXT DEFAULT 'NULL', status INTEGER); CREATE TABLE Collections (name TEXT, urlname TEXT PRIMARY KEY, torrentcount INTEGER, totalsizebytes BIGINT); CREATE TABLE Collections2Torrents (infohash TEXT, urlname TEXT, FOREIGN KEY(infohash) REFERENCES Torrents(infohash), FOREIGN KEY(urlname) REFERENCES Collections(urlname));"
*/
#define DATABASE_NAME "torrents.db"

using namespace std;

class Database

{
public:
    Database(char* filename);
    ~Database();
    bool open(char* filename);
    vector<vector<string> > query(const string query);
    void close();
private:
    sqlite3 *database;
};
#endif

