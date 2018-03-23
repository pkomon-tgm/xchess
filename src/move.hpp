/*
 * move.hpp
 *
 *  Created on: Mar 6, 2018
 *      Author: pkomon
 */

#ifndef CHESS_MOVE_HPP
#define CHESS_MOVE_HPP

#include "piece.hpp"
#include "position.hpp"

namespace chess {

	struct move {
		//piece* p;
		position source;
		position target;

		//move(const position& source, const position& target);
	};

	class invalid_move_error : std::exception { //todo add way to provide more informatio
		move m;
		std::string msg;
	public:
		invalid_move_error(const move& m, const std::string& msg);
		virtual const char* what() const noexcept override;
	};

} /* namespace chess */

#endif /* CHESS_MOVE_HPP */
