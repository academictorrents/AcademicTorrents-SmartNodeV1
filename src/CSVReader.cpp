/*
 * CSVReader.cpp
 *
 *  Created on: Feb 18, 2014
 *      Author: grigorii
 */

#include "CSVReader.h"
#include "TorrentInfo.h"
#include "database.h"

CSVReader::CSVReader(string fileContent) {
	CSVReader::fileContent = fileContent;
//	CSVReader::dataBaseName = dataBaseName;

}
string CSVReader::getfileContent() {
	return CSVReader::fileContent;
}

//char* CSVReader::getDataBaseName(){
//	return CSVReader::dataBaseName;
//}

void CSVReader::readAll() {
	string line;

	vector<string> fields;

	stringstream mydata;
	mydata<<fileContent;
	database *db;
	db = new database();

		int i = 0;

		while (getline(mydata, line)) {
			if (i != 0) {
				boost::split(fields, line, boost::is_any_of(","));

				//TorrentInfo tor(fields);
				//adding a torrent info to the data base
				//(type, name,infohash,sizebytes,mirrors,downloaders,timescompleted,dateadded,datemodified);
				string q="INSERT OR REPLACE INTO Torrents (type, name,infohash,sizebytes,mirrors,downloaders,timescompleted,dateadded,datemodified) VALUES (";
				for (size_t j=0;j<fields.size()-1;j++)
					if (j<3 || j>6 )
					q+="'"+fields[j]+"'"+",";
					else
					q+=fields[j]+",";

				//q+=")";
				q+=fields[fields.size()-1]+");";
				//q.replace(q.length()-1,2,");");
				db->query(&q[0]);
			}
			i++;
		}

        db->close();

}
CSVReader::~CSVReader() {}

