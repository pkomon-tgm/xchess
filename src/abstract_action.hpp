/*
 * abstract_action.hpp
 *
 *  Created on: Apr 12, 2018
 *      Author: pkomon
 */

#ifndef ABSTRACT_ACTION_HPP_
#define ABSTRACT_ACTION_HPP_

namespace chess {

	class board; //fw

	struct abstract_action{
		virtual ~abstract_action() = default;
		virtual void execute(board& b) = 0;
		virtual void undo(board& b) = 0;
	};

} //namespace chess

#endif /* ABSTRACT_ACTION_HPP_ */
