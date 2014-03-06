#include <iostream>
#include "CSVReader.h"
using namespace std;

int main() {
	//cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	CSVReader r("/home/grigorii/Desktop/torrents.csv");
	//CSVReader r("www.academictorrents.com/collection/uci-machine-learning-datasets.csv");
	//r.readAll();
    r.readAll();

	return 0;
}
