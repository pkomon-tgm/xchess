/*
 * server_callback.hpp
 *
 *  Created on: Apr 27, 2018
 *      Author: pkomon
 */

#ifndef SERVER_SERVER_CALLBACK_HPP_
#define SERVER_SERVER_CALLBACK_HPP_

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "player.hpp"

namespace chess {

	class game_server; //fw
	using generic_server_callback_func = std::function<void(game_server& gs, player& con, const std::vector<std::string>& args)>;

	struct server_callback {
		virtual ~server_callback() = default;
		virtual void cb(game_server& gs, player& con, const std::vector<std::string>& args) = 0;
	};

	struct generic_server_callback : public server_callback {
		generic_server_callback_func f;
		generic_server_callback(const generic_server_callback_func& f);
		virtual void cb(game_server& gs, player& con, const std::vector<std::string>& args) override;
	};

	struct server_callback_entry {
		std::string cmd;
		std::unique_ptr<server_callback> callback;

		server_callback_entry(const std::string& cmd, server_callback* cb);
		server_callback_entry(server_callback_entry&& other);
		~server_callback_entry() = default;
	};


}

#endif /* SERVER_SERVER_CALLBACK_HPP_ */
