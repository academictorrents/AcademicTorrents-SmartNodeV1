/*
 * CSVReader.cpp
 *
 *  Created on: Feb 18, 2014
 *      Author: grigorii
 */

#include "CSVReader.hpp"


CSVReader::CSVReader(string fileContent,char* dataBaseName) {
	CSVReader::fileContent = fileContent;
	CSVReader::dataBaseName = dataBaseName;

}

string CSVReader::getfileContent() {
	return CSVReader::fileContent;
}

char* CSVReader::getDataBaseName(){
	return CSVReader::dataBaseName;
}

void CSVReader::readAll() {
	string line;

	vector<string> fields;
//
	stringstream mydata;
	mydata<<fileContent;
	Database *db;
	db = new Database(dataBaseName);

    db->query(TORRENTS_TABLE);

		int i = 0;

		while (getline(mydata, line)) {
			if (i != 0) {
				boost::split(fields, line, boost::is_any_of(","));
				//TODO REMOVE TEST CODE
				/* TorrentInfo tor(fields); */
				/* adding a torrent info to the data base */
				string q="INSERT OR IGNORE INTO Torrents VALUES(";
				for (size_t j=0;j<fields.size();j++)
					if (j<3 || j>6)
					q+="'"+fields[j]+"'"+",";
					else
					q+=fields[j]+",";

				q+="'');";
				//TODO REMOVE TEST CODE
				/* q.replace(q.length()-1,2,");"); */
				db->query(&q[0]);
			}
			i++;
		}

        db->close();

}

void CSVReader::readCollections(){
	string line;

		vector<string> fields;
	//
		stringstream mydata;
		mydata<<fileContent;
		Database *db;
		db = new Database(dataBaseName);

	    db->query(TORRENTS_TABLE);

			int i = 0;

			while (getline(mydata, line)) {
				if (i != 0) {
					boost::split(fields, line, boost::is_any_of(","));
					//TorrentInfo tor(fields);
					//adding a torrent info to the data base
					string q="INSERT OR IGNORE INTO Collections VALUES(";
					for (size_t j=0;j<fields.size();j++)
						if (j<3 || j>6)
						q+=""+fields[j]+""+",";
						else
						q+=fields[j];

					q+=");";
					//q.replace(q.length()-1,2,");");
					db->query(&q[0]);
				}
				i++;
			}

	        db->close();
}
CSVReader::~CSVReader() {}

