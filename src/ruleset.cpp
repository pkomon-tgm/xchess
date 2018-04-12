/*
 * ruleset.cpp
 *
 *  Created on: Mar 6, 2018
 *      Author: pkomon
 */

#include "ruleset.hpp"

#include "piece.hpp"
#include "board.hpp"
#include "abstract_action.hpp"

#include <iostream>

namespace chess {

	ruleset::ruleset(const std::vector<rule*>& rules_before_move, const std::vector<rule*>& rules_after_move): rules_before_move{}, rules_after_move{}, next{nullptr} {
		this->rules_before_move.reserve(rules_before_move.size());
		for(std::size_t i = 0; i < rules_before_move.size(); ++i)
			this->rules_before_move.emplace_back(rules_before_move[i]);
		this->rules_after_move.reserve(rules_after_move.size());
		for(std::size_t i = 0; i < rules_after_move.size(); ++i)
			this->rules_after_move.emplace_back(rules_after_move[i]);
	}

	void ruleset::apply(board& b, const move& m, bool eval) {
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
			} catch (const invalid_move_error& e) {
				next->undo(b);
				next.reset();
				throw e;
			}
			if (!eval) {
				actions.emplace_back(std::move(next));
				after_move(b, m);
			} else {
				next->undo(b);
			}
			next.reset();
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

	void ruleset::set_next(abstract_action* action){
		next.reset(action);
	}

} /* namespace chess */
