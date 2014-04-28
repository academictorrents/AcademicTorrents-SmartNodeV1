/*
 /*
 * CSVReader.hpp
 *
 *  Created on: Feb 18, 2014
 *      Author: grigorii
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"
#include <boost/asio.hpp>
#include "TorrentInfo.hpp"
#include <boost/lexical_cast.hpp>
#include "database.hpp"

using namespace std;
#ifndef CSVREADER_H_
#define CSVREADER_H_

class CSVReader {
public:
	CSVReader(string,char*);
	string getfileContent();
	char* getDataBaseName();
	void readAll();
	void readCollections();
	~CSVReader();


private:
	string fileContent;
    char* dataBaseName;

};

#endif /* CSVREADER_H_ */
