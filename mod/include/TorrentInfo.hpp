/*
 * TorrentInfo.h
 *
 *  Created on: Feb 18, 2014
 *      Author: grigorii
 */
#include <iostream>
 #include <vector>
#include <string>
#include <stdlib.h>
#include <sstream>
using namespace std;
#ifndef TORRENTINFO_H_
#define TORRENTINFO_H_

class TorrentInfo {
public:
	TorrentInfo(vector <string>);

	~TorrentInfo();


	 string  getDateadded() ;
	int getDownloaders() const;
	string  getInfohash() const;
	int getMirrors() const;
	 string  getName() const;
	long getSizebytes() const;
	int getTimescompleted() const;
	 string  getType() const;
	 string  getAllInfo() ;

	//getters

private:
	     string type,name,infohash,dateadded;
		 long sizebytes;
		 int mirrors,downloaders,timescompleted;
		//TYPE,NAME,INFOHASH,SIZEBYTES,MIRRORS,DOWNLOADERS,TIMESCOMPLETED,DATEADDED
};

#endif /* TORRENT_H_ */
