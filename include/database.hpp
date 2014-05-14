#ifndef __DATABASE_H__
#define __DATABASE_H__
#include <string>
#include <vector>
#include <iostream>
//#include <list>           // std::list
//#include <queue>          // std::queue
#include <boost/filesystem.hpp>
#include <boost/heap/priority_queue.hpp>
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

class query_str{
	public:
	string command;
	vector<vector<string> > *result;
	bool returned;
	int priority;

	query_str();

	int operator<(const query_str other) const;
	int operator>(const query_str other) const;
	void swap(query_str& obj);
	query_str& operator=(query_str& a);
};

//typedef boost::heap::priority_queue<query_str> MyPriQue;

enum status {
  DOWNLOAD = 0, SEED = 1, WANT = 2, BANNED = 3, UNKNOWN = 4, ERROR = 5, REMOVE = 6
};

//class Query_Queue{
//public:
//	Query_Queue();
//	void addquery(query_str*);
//	void makequery();
//private:
//	MyPriQue query_Queue;
//};

class Database

{
public:

	typedef boost::heap::priority_queue<query_str> MyPriQue;

	Database();
	~Database();
	bool open(char* filename);

	vector<vector<string> > query(const string query);
	vector<string> getColNames();
	void close();

	void addquery(query_str*);
	void addquery(query_str);
	void makequery();
	bool queue_empty();
	void updateTableInfo();
	void updateTorrents(string);
	void updateCollectionsList();

private:

	sqlite3 *database;
	sqlite3_stmt *statement;
	vector<string> columns;
	MyPriQue query_Queue;

};

extern Database *db;

#endif

