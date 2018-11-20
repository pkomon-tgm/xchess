/*
 * player.hpp
 *
 *  Created on: Apr 25, 2018
 *      Author: pkomon
 */

#ifndef SERVER_PLAYER_HPP_
#define SERVER_PLAYER_HPP_

#include <string>
#include <chrono>
#include <memory>
#include <vector>

#include "client_connection.hpp"

namespace chess {

	class game_session;
	class client_connection;

	class player {
		std::string name;
		std::chrono::time_point<std::chrono::system_clock> online_since;

		std::unique_ptr<client_connection> con;
		std::vector<game_session*> part_of;

		game_session* active;

	public:
		player(const std::string& name, client_connection* con);
//		player(const std::string& name);

		const std::string& get_name() const;
		bool is_playing() const;

		client_connection& get_client_connection();
		game_session* get_active_session();
		void set_active_session(game_session& session);

		bool operator==(const player& other) const;
		bool operator!=(const player& other) const;

	};

}

#endif /* SERVER_PLAYER_HPP_ */
