/*
 * server_callback.cpp
 *
 *  Created on: Apr 27, 2018
 *      Author: pkomon
 */

#include "server_callback.hpp"

#include <vector>
#include <string>

#include "game_server.hpp"

namespace chess {

	generic_server_callback::generic_server_callback(const generic_server_callback_func& f): f{f} {}
	void generic_server_callback::cb(game_server& gs, player& con, const std::vector<std::string>& args){
		f(gs, con, args);
	}

	server_callback_entry::server_callback_entry(const std::string& cmd, server_callback* cb): cmd{cmd}, callback{cb} {}
	server_callback_entry::server_callback_entry(server_callback_entry&& other): cmd{other.cmd}, callback{std::move(other.callback)} {}

}
