/*
 * game_server.hpp
 *
 *  Created on: Apr 16, 2018
 *      Author: pkomon
 */

#ifndef SERVER_GAME_SERVER_HPP_
#define SERVER_GAME_SERVER_HPP_

#include <memory>
#include <map>
#include <string>
#include <functional>
#include <vector>

#include "boost/asio/io_context.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/enable_shared_from_this.hpp"

#include "client_connection.hpp"
#include "player.hpp"

#include "challenge.hpp"
#include "server_callback.hpp"

namespace chess {

	class game_server {
		static const std::string GREETING;
		static const char COMMAND_PREFIX;
		boost::asio::ip::tcp::acceptor acceptor;
		//std::vector<boost::asio::ip::>
		//std::map<std::string, client_connection> clients;

		std::vector<std::shared_ptr<player>> players;
		//std::vector<client_connection> connection;

		std::vector<server_callback_entry> callbacks;
		std::unique_ptr<server_callback> default_callback;

		challenge_mgr chal_mgr;

	public:
		game_server(boost::asio::io_context& context, unsigned int port);
		void start_accept();
		void start_handle_client_msg(std::shared_ptr<boost::asio::ip::tcp::socket> socket, std::shared_ptr<player> p);
		void process_msg(player& p, const std::string& msg);

		bool is_name_taken(const std::string& name);
		player& get_player(const std::string& name);
		challenge_mgr& get_challenge_mgr();

		std::vector<std::shared_ptr<player>>& get_players();
	private:
		//void accept_handler(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const boost::system::error_code& e);

		void message_handler(client_connection& con, const boost::system::error_code& e);

	};

}


#endif /* SERVER_GAME_SERVER_HPP_ */
