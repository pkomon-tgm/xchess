/*
 * game.hpp
 *
 *  Created on: Mar 8, 2018
 *      Author: pkomon
 */

#ifndef GAME_HPP
#define GAME_HPP

#include <memory>

#include "board.hpp"
#include "piece.hpp"

namespace chess {

	class game {
		board b;
		std::unique_ptr<ruleset> rules;

	public:
		game(ruleset* rules);

		void make_move(const move& m);
		board& get_board();
		ruleset& get_ruleset();
	};

} /* namespace chess */

#endif /* GAME_HPP */
