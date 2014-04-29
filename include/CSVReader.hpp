/*
 /*
 * CSVReader.h
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
using namespace std;
#ifndef CSVREADER_H_
#define CSVREADER_H_

class CSVReader {
public:
	CSVReader(string);
	string getfileContent();
//	char* getDataBaseName();
	void readAll();
	~CSVReader();


private:
    string fileContent;
//    char* dataBaseName;

};

#endif /* CSVREADER_H_ */
