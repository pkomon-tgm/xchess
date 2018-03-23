/*
 * position.hpp
 *
 *  Created on: Mar 6, 2018
 *      Author: pkomon
 */

#ifndef CHESS_POSITION_HPP
#define CHESS_POSITION_HPP

#include "boost/functional/hash.hpp"

namespace chess {

	struct position {
		int x;
		int y;

		position(char x, int y);
		position(int x, int y);

		bool operator==(const position& other) const;
		bool operator!=(const position& other) const;

		position& operator+=(const position& other);
		position operator+(const position& other) const;
		position operator-(const position& other) const;
		position operator*(int other) const;
		position operator*=(int other);

		char get_x_char() const; //maps coords to chars (like on a real board)

		static char to_char(int i);
		static int to_index(char c);

	};

} /* namespace chess */

namespace std {

	template<> struct hash<chess::position> {
		std::size_t operator()(const chess::position& key) const {
			std::size_t seed = 0;

			boost::hash_combine(seed, boost::hash_value(key.x));
			boost::hash_combine(seed, boost::hash_value(key.y));

			return seed;
		}
	};

} /* namespace std */

#endif /* CHESS_POSITION_HPP */
