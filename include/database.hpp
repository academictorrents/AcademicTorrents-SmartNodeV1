#ifndef __DATABASE_H__
#define __DATABASE_H__
#include <string>
#include <vector>
#include <sqlite3.h>

//#define TORRENTS_TABLE "create table if not exists Torrents (type TEXT, name TEXT,infohash TEXT  PRIMARY KEY,sizebytes BIGINT,mirrors INTEGER,downloaders INTEGER,timescompleted INTEGER,dateadded TEXT,datemodified TEXT, torrentpath TEXT DEFAULT 'NULL');"

#define TORRENTS_TABLE "create table if not exists Torrents (type TEXT, name TEXT,infohash TEXT  PRIMARY KEY,sizebytes BIGINT,mirrors INTEGER,downloaders INTEGER,timescompleted INTEGER,\
dateadded TEXT,datemodified TEXT, torrentpath TEXT DEFAULT 'NULL', bibtex TEXT DEFAULT 'NULL',status INTEGER DEFAULT 0);"

#define COLLECTIONS_TABLE "create table if not exists Collections (name TEXT, urlname TEXT PRIMARY KEY, torrentcount INTEGER, totalsizebytes BIGINT);"

#define COLLECTIONS2TORRENTS_TABLE "create table if not exists Collections2Torrents (infohash TEXT, urlname TEXT, FOREIGN KEY(infohash) REFERENCES Torrents(infohash), FOREIGN KEY(urlname) REFERENCES Collections(urlname));"

#define DATABASE_NAME "torrents.db"

using namespace std;

class database

{
public:
    database();
    ~database();
    bool open(char* filename);
    vector<vector<string> > query(char* query);
    void close();
private:
    sqlite3 *data_base;
};
#endif

