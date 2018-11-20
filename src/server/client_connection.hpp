/*
 * client_connection.hpp
 *
 *  Created on: Apr 18, 2018
 *      Author: pkomon
 */

#ifndef SERVER_CLIENT_CONNECTION_HPP_
#define SERVER_CLIENT_CONNECTION_HPP_

#include <vector>

#include "boost/asio/ip/tcp.hpp"

namespace chess {

	class client_connection {
		//std::string name;

		std::shared_ptr<boost::asio::ip::tcp::socket> socket;

	public:
		client_connection(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

		boost::asio::ip::tcp::socket& get_socket();
	};

}

#endif /* SERVER_CLIENT_CONNECTION_HPP_ */
