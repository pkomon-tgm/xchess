/*
 * player.cpp
 *
 *  Created on: Apr 25, 2018
 *      Author: pkomon
 */

#include "player.hpp"

#include <string>
#include <vector>
#include <chrono>

namespace chess {


	player::player(const std::string& name, client_connection* con):
			name{name}, online_since{std::chrono::system_clock::now()}, con{con}, active{nullptr} {};
//	player::player(const std::string& name): player{name, nullptr} {};

	const std::string& player::get_name() const {return name;}
	bool player::is_playing() const {return !part_of.empty();}
	client_connection& player::get_client_connection(){return *con;}
	game_session* player::get_active_session(){return active;}
	void player::set_active_session(game_session& session){active = &session;}
	bool player::operator==(const player& other) const {return other.get_name() == name;}
	bool player::operator!=(const player& other) const {return !(other.get_name() == name);}



}


