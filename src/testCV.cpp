#include <iostream>
#include "CSVReader.h"
#include "database.h"
using namespace std;

int main() {
	//cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	std::ifstream ifs("torrents.csv");
	  std::string content( (std::istreambuf_iterator<char>(ifs) ),
	                       (std::istreambuf_iterator<char>()    ) );
	CSVReader r(content);


	std::ifstream ifs2("torrents2.csv");
		  std::string content2( (std::istreambuf_iterator<char>(ifs2) ),
		                       (std::istreambuf_iterator<char>()    ) );
	CSVReader r2(content2);
	//CSVReader r("www.academictorrents.com/collection/uci-machine-learning-datasets.csv");
	//r.readAll();
    r.readAll();
    r2.readAll();

cout<<"Data from Data base:"<<endl;
database *db;
db = new database();

//vector<vector<string> > result = db->query("SELECT type, name FROM Torrents;");
vector<vector<string> > result = db->query("select * FROM Torrents WHERE mirrors=10;");

vector<vector<string> > result2 = db->query("select * FROM Torrents;");
cout<<result.size()<<endl;
for(vector<vector<string> >::iterator it = result.begin(); it < result.end(); ++it)
{
    vector<string> row = *it;
    cout << "Values: (Type=" << row.at(0) << ", Name=" << row.at(1) << ")" << endl;
}

//for(vector<vector<string> >::iterator it = result2.begin(); it < result2.end(); ++it)
//{
//    vector<string> row = *it;
//    cout << "Values: (Type=" << row.at(0) << ", Name=" << row.at(1) << ")" << endl;
//}

cout<<"Last result"<<endl;

cout<<result2[5][1]<<endl;

db->close();
	return 0;
}
