/*
 * ruleset.cpp
 *
 *  Created on: Mar 6, 2018
 *      Author: pkomon
 */

#include "ruleset.hpp"

#include "piece.hpp"
#include "board.hpp"

namespace chess {

	rule::rule(const std::set<piece_type>& applies_to_types): applies_to_types{applies_to_types} {}

	bool rule::applies_to(piece_type type) const {return applies_to_types.find(type) != applies_to_types.end();}
	void rule::apply(ruleset& rules, board& b, const move& m) {
		if(applies_to(b.at(m.source).get_type()))
			rule_callback(rules, b, m);
	}


	generic_rule::generic_rule(const std::set<piece_type>& applies_to_types, const generic_rule_cb_function& cb): rule{applies_to_types}, cb{cb} {}
	void generic_rule::rule_callback(ruleset& rules, board& b, const move& m) {
		cb(rules, b, m, b.at(m.source));
	}


	ruleset::ruleset(const std::vector<rule*>& rules): rules{} {
		this->rules.reserve(rules.size());
		for(std::size_t i = 0; i < rules.size(); ++i)
			this->rules.emplace_back(rules[i]);
	}

	void ruleset::apply(board& b, const move& m) {
		for(auto& r : rules){
			//check rule application in ruleset or rule?
			//if(r.applies_to(m.p->get_type()))
			//	r.cb(b, m);
			r->apply(*this, b, m);
		}
	}

	void ruleset::init(board& b){}
	void ruleset::reset(board& b){
		b.reset_pieces();
		init(b);
	}

} /* namespace chess */
