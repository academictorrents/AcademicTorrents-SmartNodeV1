///*
// * boost_test.cpp
// *
// *  Created on: Feb 22, 2014
// *      Author: nogueira
// */
//#include <iostream>
//#include <iterator>
//#include <boost/iostreams/tee.hpp>
//#include "boost/iostreams/stream.hpp"
//#include <fstream>
//#include <iostream>
//
//#include "libtorrent/session.hpp"
//#include "libtorrent/session_settings.hpp"
//#include "libtorrent/hasher.hpp"
//#include "libtorrent/create_torrent.hpp"
//#include <boost/thread.hpp>
//#include <boost/tuple/tuple.hpp>
//
//#include "test.hpp"
//#include "setup_transfer.hpp"
//
//using std::ostream;
//using std::ofstream;
//using std::cout;
//
////boost
//using boost::iostreams::tee_device;
//using boost::iostreams::stream;
//
//////libtorrent
//using namespace libtorrent;
//
//
//
//class testboost{
//	public:
//		 testboost(){
//			    typedef tee_device<ostream, ofstream> TeeDevice;
//			    typedef stream<TeeDevice> TeeStream;
//			    ofstream ofs("sample.txt");
//			    TeeDevice my_tee(cout, ofs);
//			    TeeStream my_split(my_tee);
//			    my_split << "testing boost\n";
//			    my_split.flush();
//			    my_split.close();
//		 }
//};
//
//class testlibtorrent{
//
//	void test_running_torrent(boost::intrusive_ptr<torrent_info> info, size_type file_size)
//	{
//		session ses(fingerprint("LT", 0, 1, 0, 0), std::make_pair(48130, 48140));
//
//		add_torrent_params p;
//		p.ti = info;
//		p.save_path = ".";
//		torrent_handle h = ses.add_torrent(p);
//
//		test_sleep(500);
//		torrent_status st = h.status();
//
//		std::cout << "total_wanted: " << st.total_wanted << " : " << file_size * 3 << std::endl;
//		TEST_CHECK(st.total_wanted == file_size * 3);
//		std::cout << "total_wanted_done: " << st.total_wanted_done << " : 0" << std::endl;
//		TEST_CHECK(st.total_wanted_done == 0);
//
//		std::vector<int> prio(3, 1);
//		prio[0] = 0;
//		h.prioritize_files(prio);
//
//		test_sleep(500);
//		st = h.status();
//
//		std::cout << "total_wanted: " << st.total_wanted << " : " << file_size * 2 << std::endl;
//		TEST_CHECK(st.total_wanted == file_size * 2);
//		std::cout << "total_wanted_done: " << st.total_wanted_done << " : 0" << std::endl;
//		TEST_CHECK(st.total_wanted_done == 0);
//
//		prio[1] = 0;
//		h.prioritize_files(prio);
//
//		test_sleep(500);
//		st = h.status();
//
//		std::cout << "total_wanted: " << st.total_wanted << " : " << file_size << std::endl;
//		TEST_CHECK(st.total_wanted == file_size);
//		std::cout << "total_wanted_done: " << st.total_wanted_done << " : 0" << std::endl;
//		TEST_CHECK(st.total_wanted_done == 0);
//		cout << "testing libtorrent";
//	}
//};
//
//int main(){
//	cout << "Starting boost lib test\n";
//	testboost t;
//	cout << "finishing boost lib test\n";
//	cout << "Starting libtorrent lib test\n";
//	testlibtorrent l;
//	cout << "finishing libtorrent test\n";
//}



