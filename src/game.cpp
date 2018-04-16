/*
 * game.cpp
 *
 *  Created on: Mar 8, 2018
 *      Author: pkomon
 */

#include "game.hpp"

namespace chess {

	game::game(ruleset* rules): b{8}, rules{rules} {
		rules->init(b);
	}


	void game::make_move(const move& m){
		if(b.is_empty(m.source))
			throw invalid_move_error(m, "no piece on source field");
		rules->apply(b, m);
	}

	board& game::get_board(){return b;}
	ruleset& game::get_ruleset(){return *rules;}


} /* namespace chess */
