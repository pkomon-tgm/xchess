/*
 * session.hpp
 *
 *  Created on: Apr 18, 2018
 *      Author: pkomon
 */

#ifndef SERVER_SESSION_HPP_
#define SERVER_SESSION_HPP_

#include "boost/asio/ip/tcp.hpp"

#include "../chess_ruleset.hpp"
#include "../game.hpp"
#include "player.hpp"

namespace chess {

	class client_connection;

	class game_session {
		game model;
		const player* challenger;
		const player* challenged;
	public:
		game_session(chess::chess_ruleset* rules, const player& challenger, const player& challenged);

		const player& get_challenger() const; //remove duplicate code in session and challenge
		const player& get_challenged() const;
		game& get_model();
	};

}

#endif /* SERVER_SESSION_HPP_ */
