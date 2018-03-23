/*
 * game.cpp
 *
 *  Created on: Mar 8, 2018
 *      Author: pkomon
 */

#include "game.hpp"

namespace chess {

	game::game(const board& b, ruleset& rules): b{b}, rules{&rules} {
		rules.init(this->b);
	}


	void game::make_move(const move& m){
		if(b.is_empty(m.source))
			throw invalid_move_error(m, "no piece on source field");
		rules->apply(b, m);

		//TODO do actual move on board! how to implement special moves, distinct to ruleset?
		//maybe execute move in rule itself?
		b.set_piece(b.at(m.source), m.target);
		b.remove_piece(m.source);
	}

	board& game::get_board(){return b;}


} /* namespace chess */
