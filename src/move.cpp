/*
 * move.cpp
 *
 *  Created on: Mar 6, 2018
 *      Author: pkomon
 */

#include "move.hpp"

#include <string>
#include <iostream>

namespace chess {

	move::move(const position& source, const position& target): source{source}, target{target} {}

	invalid_move_error::invalid_move_error(const move& m, const std::string& msg): m{m}, msg{msg} {}

	const char* invalid_move_error::what() const noexcept{
		//std::string out = std::string("Invalid move: ") + std::to_string(m.source.get_x_char()) + std::to_string(m.source.y) + std::string("->") + std::to_string(m.target.get_x_char()) + std::to_string(m.target.y) + '\n';
		//out += msg;
		return msg.c_str();
	}

} /* namespace chess */
