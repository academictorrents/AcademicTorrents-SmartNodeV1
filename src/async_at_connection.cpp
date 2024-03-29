/*
 * async_at_connection.cpp
 *
 *  Created on: Feb 28, 2014
 *      Author: nogueira
 */

#include "async_at_connection.hpp"

using namespace async_at_client;

	  async_at_connection::async_at_connection(boost::asio::io_service& io_service,
		  const std::string& server, const std::string& path, void(*callback)(boost::asio::streambuf *, string s), string col)
		: resolver_(io_service),
		  socket_(io_service)
	  {
		 /*
		  * Form the request. We specify the "Connection: close" header so that the
		  *	server will close the socket after transmitting the response. This will
		  * allow us to treat all data up until the EOF as the content.
		  * */
		callback_func = callback;
		collection = col;
		std::ostream request_stream(&request_);
				request_stream << "GET " << path << " HTTP/1.0\r\n";
				request_stream << "Host: " << server << "\r\n";
				request_stream << "Accept: */*\r\n";
				request_stream << "Connection: close\r\n\r\n";
				/*
				 * Start an asynchronous resolve to translate the server and service names
				 * into a list of endpoints.
				 * */
				tcp::resolver::query query(server, "http");
				resolver_.async_resolve(query,
					boost::bind(&async_at_connection::handle_resolve, this,
					  boost::asio::placeholders::error,
					  boost::asio::placeholders::iterator));
	  }

	  void async_at_connection::handle_resolve(const boost::system::error_code& err,
		  tcp::resolver::iterator endpoint_iterator)
	  {
		if (!err)
		{
		/*
		 *Attempt a connection to each endpoint in the list until we
		 *successfully establish a connection.
		 **/
		  boost::asio::async_connect(socket_, endpoint_iterator,
			  boost::bind(&async_at_connection::handle_connect, this,
				boost::asio::placeholders::error));
		}
		else
		{
		  std::cout << "Error: " << err.message() << "\n";
		}
	  }

	  void async_at_connection::handle_connect(const boost::system::error_code& err)
	  {
		if (!err)
		{
		   /*The connection was successful. Send the request.*/
		  boost::asio::async_write(socket_, request_,
			  boost::bind(&async_at_connection::handle_write_request, this,
				boost::asio::placeholders::error));
		}
		else
		{
		  std::cout << "Error: " << err.message() << "\n";
		}
	  }

	  void async_at_connection::handle_write_request(const boost::system::error_code& err)
	  {
		if (!err)
		{
		  /*
		   * Read the response status line. The response_ streambuf will
		   * automatically grow to accommodate the entire line. The growth may be
		   * limited by passing a maximum size to the streambuf constructor.
		   * */
		  boost::asio::async_read_until(socket_, response_, "\r\n",
			  boost::bind(&async_at_connection::handle_read_status_line, this,
				boost::asio::placeholders::error));
		}
		else
		{
		  std::cout << "Error: " << err.message() << "\n";
		}
	  }

	  void async_at_connection::handle_read_status_line(const boost::system::error_code& err)
	  {
		if (!err)
		{
		  /* Check that response is OK. */
		  std::istream response_stream(&response_);
		  std::string http_version;
		  response_stream >> http_version;
		  unsigned int status_code;
		  response_stream >> status_code;
		  std::string status_message;
		  std::getline(response_stream, status_message);
		  if (!response_stream || http_version.substr(0, 5) != "HTTP/")
		  {
			std::cout << "Invalid response\n";
			return;
		  }
		  if (status_code != 200)
		  {
			std::cout << "Response returned with status code ";
			std::cout << status_code << "\n";
			return;
		  }

		  /* Read the response headers, which are terminated by a blank line. */
		  boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
			  boost::bind(&async_at_connection::handle_read_headers, this,
				boost::asio::placeholders::error));
		}
		else
		{
		  std::cout << "Error: " << err << "\n";
		}
	  }

	  void async_at_connection::handle_read_headers(const boost::system::error_code& err)
	  {
		if (!err)
		{
		  /* Process the response headers.*/
		  std::istream response_stream(&response_);
		  std::string header;
		  while (std::getline(response_stream, header) && header != "\r")
			std::cout << header << "\n";
		  std::cout << "\n";

		  /* Write whatever content we already have to output.*/
		  if (response_.size() > 0)

		  /* Start reading remaining data until EOF.*/
		  boost::asio::async_read(socket_, response_,
			  boost::asio::transfer_at_least(1),
			  boost::bind(&async_at_connection::handle_read_content, this,
				boost::asio::placeholders::error));
		}
		else
		{
		  std::cout << "Error: " << err << "\n";
		}
	  }

	  void async_at_connection::handle_read_content(const boost::system::error_code& err)
	  {
		if (!err)
		{
		  // Continue reading remaining data until EOF.
		  boost::asio::async_read(socket_, response_,
			  boost::asio::transfer_at_least(1),
			  boost::bind(&async_at_connection::handle_read_content, this,
				boost::asio::placeholders::error));
		}
		else if (err != boost::asio::error::eof)
		{
		  std::cout << "Error: " << err << "\n";
		}
		else if(err == boost::asio::error::eof)
		{
			  // make call to callback at eof
				callback_func(&response_, collection);
		}
	  }

//int main()
//{
//  try
//  {
//    boost::asio::io_service io_service;
//    async_at_connection c(io_service, "www.academictorrents.com", "/download/cb1655a57dd24345c9ea7a43c5ec09e03c7a0979.torrent"); //  /collection/umass-boston-cs-department.csv /collections.php?format=.csv
//    io_service.run();
//  }
//  catch (std::exception& e)
//  {
//    std::cout << "Exception: " << e.what() << "\n";
//  }
//
//  return 0;
//}
