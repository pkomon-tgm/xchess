/*
 * game.hpp
 *
 *  Created on: Mar 8, 2018
 *      Author: pkomon
 */

#ifndef GAME_HPP
#define GAME_HPP

#include <exception>

#include "board.hpp"
#include "piece.hpp"

namespace chess {

	class game {
		board b;
		ruleset* rules;

	public:
		game(const board& b, ruleset& rules);

		void make_move(const move& m);
		board& get_board();
	};

} /* namespace chess */

#endif /* GAME_HPP */
