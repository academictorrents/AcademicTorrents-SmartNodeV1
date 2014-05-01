/*
 * at_socket.hpp
 *
 *  Created on: Feb 28, 2014
 *      Author: nogueira
 */
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#ifndef AT_SOCKET_HPP_
#define AT_SOCKET_HPP_
#define AT_URL "www.academictorrents.com"

namespace async_at_client
{

	class async_at_connection;

	using namespace std;
	using boost::asio::io_service;
	using boost::asio::ip::tcp;
	using boost::asio::streambuf;


	class async_at_connection{

		tcp::resolver resolver_;
		tcp::socket socket_;
		boost::asio::streambuf request_;
		boost::asio::streambuf response_;

		string server;
		string path;
		string collection;

		void (*callback_func)(boost::asio::streambuf *,string);

		public:
			async_at_connection(boost::asio::io_service&, const std::string&,const std::string&, void (*callback)(boost::asio::streambuf *, string), string);
			int connect();
			string getCSV();

		private:
			int url_encoder(string);
			void handle_resolve(const boost::system::error_code&,tcp::resolver::iterator);
			void handle_connect(const boost::system::error_code&);
			void handle_write_request(const boost::system::error_code&);
			void handle_read_status_line(const boost::system::error_code&);
			void handle_read_headers(const boost::system::error_code&);
			void handle_read_content(const boost::system::error_code&);

	};

}

#endif /* AT_SOCKET_HPP_ */
