/*
 * CSVReader.cpp
 *
 *  Created on: Feb 18, 2014
 *      Author: grigorii
 */

#include "CSVReader.h"
#include "TorrentInfo.h"

CSVReader::CSVReader(string file)
    {
	CSVReader::filepath=file;

		   //"/home/grigorii/Desktop/car.txt"

    }
string CSVReader::getpath()
	{
	return CSVReader::filepath;
	}
void CSVReader::readAll(){
	string line;
	//string p=""+CSVReader::filepath;
    vector <string> fields;
			   //"/home/grigorii/Desktop/car.txt"
	    ifstream myfile (CSVReader::filepath.c_str(),std::ifstream::in);
			   if (myfile.is_open())
			   {
				   int i=0;
			     while ( getline (myfile,line) )
			     {
			    	 if(i!=0)
			    	 {
			    	  boost::split(fields,line,boost::is_any_of(","));
			     	  //boost::split( fields, line, (","));
			    	  TorrentInfo tor(fields);
			     	  //for (size_t i = 0; i < fields.size(); i++)
			    	  cout <<"Torrent" <<i<<":"<<'\n'<< tor.getAllInfo() << endl;
			    	  cout <<'\n';
			    	 }
			    	 i++;
			      }
			     myfile.close();

			   }

			   else cout << "Unable to open file";
}




CSVReader::~CSVReader(){};

