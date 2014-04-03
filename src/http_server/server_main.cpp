//
// main.cpp
//

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "server.hpp"

int main(int argc, char* argv[])
{
  try
  {

    // Initialise the server.
    std::size_t num_threads = boost::lexical_cast<std::size_t>(2);
    http::at_server::server s("127.0.0.1", "3000", "./public_html", num_threads);

    // Run the server until stopped.
    s.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}
