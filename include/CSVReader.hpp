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
 //#include "StringSplit.hpp"
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"
using namespace std;
#ifndef CSVREADER_H_
#define CSVREADER_H_

class CSVReader {
public:
	CSVReader(string);
	string getpath();
	void readAll();
	~CSVReader();


//private:
    string filepath;

};

#endif /* CSVREADER_H_ */
