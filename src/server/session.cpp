/*
 * session.cpp
 *
 *  Created on: Apr 26, 2018
 *      Author: pkomon
 */

#include "session.hpp"

#include "../chess_ruleset.hpp"

namespace chess {

	game_session::game_session(chess::chess_ruleset* rules, const player& challenger, const player& challenged):
		model{rules}, challenger{&challenger}, challenged(&challenged) {}


	const player& game_session::get_challenger() const {return *challenger;}

	const player& game_session::get_challenged() const {return *challenged;}

	game& game_session::get_model(){return model;}
}


