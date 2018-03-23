/*
 * board.hpp
 *
 *  Created on: Jan 17, 2018
 *      Author: pkomon
 */

#ifndef CHESS_BOARD_HPP
#define CHESS_BOARD_HPP

#include <memory>
#include <unordered_map>

#include "piece.hpp"
#include "position.hpp"
#include "move.hpp"
#include "ruleset.hpp"

namespace chess {

	class board {
	private:
		std::unordered_map<position, piece> fields;

		unsigned short size;

		unsigned short to_index(const position& pos);
	public:
		board(unsigned short size=8);

		void reset_pieces();

		void set_piece(const piece& p, const position& pos);
		void remove_piece(const position& pos);

		piece& at(const position& pos);
		const piece& at(const position& pos) const;
		bool is_empty(const position& pos) const;
		bool is_in_bounds(const position& pos) const;

		//for debugging purposes only
		std::string to_string();
	};

}

#endif /* CHESS_BOARD_HPP */
