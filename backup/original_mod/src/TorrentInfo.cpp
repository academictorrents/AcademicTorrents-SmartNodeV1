/*
 * TorrentInfo.cpp
 *
 *  Created on: Feb 18, 2014
 *      Author: grigorii
 */

#include "TorrentInfo.hpp"

TorrentInfo::TorrentInfo(vector <string> TorrentInfoinfo) {
	// TODO Auto-generated constructor stub
	type=TorrentInfoinfo[0];
	name=TorrentInfoinfo[1];
	infohash=TorrentInfoinfo[2];
	sizebytes=atol(TorrentInfoinfo[3].c_str());
	mirrors=atoi(TorrentInfoinfo[4].c_str());
	downloaders=atoi(TorrentInfoinfo[5].c_str());
	timescompleted=atoi(TorrentInfoinfo[6].c_str());
	dateadded=TorrentInfoinfo[7];
}

    string  TorrentInfo::getDateadded()  {
		return TorrentInfo::dateadded;
	}

	int TorrentInfo::getDownloaders() const {
		return TorrentInfo::downloaders;
	}

	 string  TorrentInfo::getInfohash() const {
		return TorrentInfo::infohash;
	}

	int TorrentInfo::getMirrors() const {
		return TorrentInfo::mirrors;
	}

	 string  TorrentInfo::getName() const {
		return TorrentInfo::name;
	}

	long TorrentInfo::getSizebytes() const {
		return TorrentInfo::sizebytes;
	}

	int TorrentInfo::getTimescompleted() const {
		return TorrentInfo::timescompleted;
	}

	 string  TorrentInfo::getType() const {
		return TorrentInfo::type;
	}

	 string  TorrentInfo::getAllInfo() {
		 stringstream s;
		 s<<TorrentInfo::type<<"  "<<TorrentInfo::name<<"  "<<TorrentInfo::infohash<<"  "<<TorrentInfo::sizebytes<<"  "
					<<TorrentInfo::mirrors<<"  "<<TorrentInfo::downloaders<<"  "<<TorrentInfo::timescompleted<<"  "<<TorrentInfo::dateadded;
		return s.str();
	 }



TorrentInfo::~TorrentInfo() {
	// TODO Auto-generated destructor stub
}

