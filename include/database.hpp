#ifndef __DATABASE_H__
#define __DATABASE_H__
#include <string>
#include <vector>
#include <iostream>
#include <sqlite3.h>
#include "at_fetcher.hpp"

#define TORRENTS_TABLE "create table if not exists Torrents (type TEXT, name TEXT,infohash TEXT  PRIMARY KEY,sizebytes BIGINT,mirrors INTEGER,downloaders INTEGER,timescompleted INTEGER,dateadded TEXT,datemodified TEXT, torrentpath TEXT DEFAULT 'NULL', filename TEXT DEFAULT 'NULL', status INTEGER DEFAULT 4, bibtex TEXT DEFAULT NULL);"
#define COLLECTIONS_TABLE "CREATE TABLE if not exists Collections (name TEXT, urlname TEXT PRIMARY KEY, torrentcount INTEGER, totalsizebytes BIGINT, mirrored INTEGER DEFAULT 0);"
#define COLLECTION2TORRENTS_TABLE "CREATE TABLE if not exists Collections2Torrents (infohash TEXT, urlname TEXT, FOREIGN KEY(infohash) REFERENCES Torrents(infohash), FOREIGN KEY(urlname) REFERENCES Collections(urlname));"

//#define DATABASE_TABLES "CREATE TABLE Torrents (type TEXT, name TEXT,infohash TEXT  PRIMARY KEY,sizebytes BIGINT,mirrors INTEGER,downloaders INTEGER,timescompleted INTEGER,
//dateadded TEXT,datemodified TEXT, torrentpath TEXT DEFAULT 'NULL', status INTEGER);
//
//-- NAME,URLNAME,TORRENTCOUNT,TOTALSIZE
//
//CREATE TABLE Collections (name TEXT, urlname TEXT PRIMARY KEY, torrentcount INTEGER, totalsizebytes BIGINT);
//
//CREATE TABLE Collections2Torrents (infohash TEXT, urlname TEXT, FOREIGN KEY(infohash) REFERENCES Torrents(infohash), FOREIGN KEY(urlname) REFERENCES Collections(urlname));"


#define DATABASE_NAME "smartnode.db"

using namespace std;

class Database

{
public:
    Database();
    ~Database();
    bool open(char* filename);
    vector<vector<string> > query(const string query);
    vector<string> getColNames();
    void close();

    void updateTableInfo();
    void updateTorrents(string);
    void updateCollectionsList();

private:
    sqlite3 *database;
    sqlite3_stmt *statement;
    vector<string> columns;
};
#endif

