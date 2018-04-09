/*
 * piece.hpp
 *
 *  Created on: Jan 17, 2018
 *      Author: pkomon
 */

#ifndef CHESS_PIECE_HPP
#define CHESS_PIECE_HPP

#include <vector>

namespace chess {

	//TODO generalize for use with other board games!

	//forward declaration
	//class board;

	//enum piece_color {BLACK, WHITE};

	class piece_color {
	private:
		int color_index;

		piece_color(int color);
	public:
		piece_color operator!() const;
		bool operator==(const piece_color& other) const;
		bool operator!=(const piece_color& other) const;

		static const piece_color WHITE;
		static const piece_color BLACK;
	};


	enum piece_type {
		PAWN, KNIGHT, BISHOP, ROCK, QUEEN, KING
	};


	class piece {
	private:
		piece_type type;
		piece_color color;

	public:
		piece(const piece_type& t, const piece_color& c);
		piece(const piece&) = default;

		piece_type get_type() const;
		piece_color get_color() const;

		bool operator==(const piece& other) const;
		bool operator!=(const piece& other) const;

	};

}

#endif /* CHESS_PIECE_HPP */
