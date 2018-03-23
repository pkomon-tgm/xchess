/*
 * position.cpp
 *
 *  Created on: Mar 6, 2018
 *      Author: pkomon
 */

#include "position.hpp"

#include <cmath>

namespace chess {

	// 65 = A in ASCII, find better solution? lol
	char position::to_char(int i){return i + 'A' - 1;};
	int position::to_index(char c){return c - 'A' + 1;};

	char position::get_x_char() const {return to_char(x);}

	bool position::operator==(const position& other) const {return x == other.x && y == other.y;}
	bool position::operator!=(const position& other) const {return !(*this == other);}
	position& position::operator+=(const position& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	position position::operator+(const position& other) const {return position{x+other.x, y+other.y};}
	position position::operator-(const position& other) const {return position{x-other.x, y-other.y};}
	position position::operator*(int other) const {return position{x*other, y*other};}
	position position::operator*=(int other) {
		x *= other;
		y *= other;
		return *this;
	}


	position::position(char x, int y): x{to_index(x)}, y{y} {};
	position::position(int x, int y): x{x}, y{y} {};

	position abs(const position& p) {return position{static_cast<short>(std::abs(p.x)), std::abs(p.y)};}

} /* namespace chess */
