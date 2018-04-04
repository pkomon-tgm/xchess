/*
 * ruleset.cpp
 *
 *  Created on: Mar 6, 2018
 *      Author: pkomon
 */

#include "ruleset.hpp"

#include "piece.hpp"
#include "board.hpp"

#include <iostream>

namespace chess {

	rule::rule(const std::set<piece_type>& applies_to_types): applies_to_types{applies_to_types} {}

	bool rule::applies_to(piece_type type) const {return applies_to_types.find(type) != applies_to_types.end();}

	bool rule::apply(ruleset& rules, board& b, const move& m, piece_type type){
		if(applies_to(type))
			return rule_callback(rules, b, m);
		return false;
	}

	bool rule::apply(ruleset& rules, board& b, const move& m) {
		return apply(rules, b, m, b.at(m.source).get_type());
	}


	generic_rule::generic_rule(const std::set<piece_type>& applies_to_types, const generic_rule_cb_function& cb): rule{applies_to_types}, cb{cb} {}
	bool generic_rule::rule_callback(ruleset& rules, board& b, const move& m) {
		return cb(rules, b, m);
	}


	ruleset::ruleset(const std::vector<rule*>& rules_before_move, const std::vector<rule*>& rules_after_move): rules_before_move{}, rules_after_move{}, next{nullptr} {
		this->rules_before_move.reserve(rules_before_move.size());
		for(std::size_t i = 0; i < rules_before_move.size(); ++i)
			this->rules_before_move.emplace_back(rules_before_move[i]);
		this->rules_after_move.reserve(rules_after_move.size());
		for(std::size_t i = 0; i < rules_after_move.size(); ++i)
			this->rules_after_move.emplace_back(rules_after_move[i]);
	}

	void ruleset::apply(board& b, const move& m) {
		bool consumed = false;
		for(std::size_t i = 0; i < rules_before_move.size() && !consumed; ++i){
			consumed |= rules_before_move[i]->apply(*this, b, m);
		}
		if(next != nullptr){
			next->execute(b);
			consumed = false;
			try {
				for(std::size_t i = 0; i < rules_after_move.size() && !consumed; ++i)
					consumed |= rules_after_move[i]->apply(*this, b, m, b.at(m.target).get_type());
			} catch (std::exception& e) {
				next->undo(b);
				throw e;
			}
			next = nullptr;
			actions.emplace_back(next);
			after_move(b, m);
		} else
			throw invalid_move_error(m, "no rule set next action, should never arise!");

	}

	void ruleset::init(board& b){}
	void ruleset::reset(board& b){
		b.reset_pieces();
		init(b);
	}
	void ruleset::after_move(board& b, const move& m){}

	std::vector<std::unique_ptr<abstract_action>>& ruleset::get_actions(){return actions;}


} /* namespace chess */
