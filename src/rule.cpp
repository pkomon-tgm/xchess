/*
 * rule.cpp
 *
 *  Created on: Apr 12, 2018
 *      Author: pkomon
 */

#include "rule.hpp"

#include "piece.hpp"
#include "move.hpp"
#include "board.hpp"

namespace chess {

	rule::rule(const std::set<piece_type>& applies_to_types): applies_to_types{applies_to_types} {}

	bool rule::applies_to(piece_type type) const {return applies_to_types.find(type) != applies_to_types.end();}

	bool rule::apply(ruleset& rules, board& b, const move& m, piece_type type){
		if(applies_to(type))
			return rule_callback(rules, b, m);
		return false;
	}


} // namespace chess
