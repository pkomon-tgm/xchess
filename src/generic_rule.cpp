/*
 * generic_rule.cpp
 *
 *  Created on: Apr 12, 2018
 *      Author: pkomon
 */

#include "generic_rule.hpp"

namespace chess {

	generic_rule::generic_rule(const std::set<piece_type>& applies_to_types, const generic_rule_cb_function& cb): rule{applies_to_types}, cb{cb} {}
	bool generic_rule::rule_callback(ruleset& rules, board& b, const move& m) {
		return cb(rules, b, m);
	}


} //namespace chess
