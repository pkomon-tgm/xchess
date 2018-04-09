/*
 * piece.cpp
 *
 *  Created on: Jan 17, 2018
 *      Author: pkomon
 */

#include "piece.hpp"

namespace chess {

	piece_color::piece_color(int color_index): color_index{color_index} {}
	piece_color piece_color::operator!() const {return *this == piece_color::WHITE ? piece_color::BLACK : piece_color::WHITE;}
	bool piece_color::operator==(const piece_color& other) const {return color_index == other.color_index;}
	bool piece_color::operator!=(const piece_color& other) const {return color_index != other.color_index;}

	const piece_color piece_color::WHITE{0};
	const piece_color piece_color::BLACK{1};

	piece::piece(const piece_type& t, const piece_color& c): type{t}, color{c} {}
	piece_color piece::get_color() const {return color;}
	piece_type piece::get_type() const {return type;}

	bool piece::operator==(const piece& other) const {return other.get_color() == color && other.get_type() == type;}
	bool piece::operator!=(const piece& other) const {return !(*this == other);}

} /* namespace chess */
