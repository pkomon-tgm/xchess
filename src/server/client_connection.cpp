/*
 * client_connection.cpp
 *
 *  Created on: Apr 19, 2018
 *      Author: pkomon
 */

#include "client_connection.hpp"

#include <memory>

#include "boost/asio/ip/tcp.hpp"

namespace chess {

	client_connection::client_connection(std::shared_ptr<boost::asio::ip::tcp::socket> socket): socket{socket} {}

	boost::asio::ip::tcp::socket& client_connection::get_socket(){return *socket;}


} // namespace chess
