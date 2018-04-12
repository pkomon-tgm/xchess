/*
 * ruleset.hpp
 *
 *  Created on: Mar 6, 2018
 *      Author: pkomon
 */

#ifndef RULESET_HPP
#define RULESET_HPP

#include <vector>
#include <set>
#include <exception>
#include <memory>
#include <functional>

#include "move.hpp"
#include "abstract_action.hpp"
#include "rule.hpp"

namespace chess {

	class board; //fw

	class ruleset {

	protected:
		std::vector<std::unique_ptr<abstract_action>> actions;
		std::vector<std::unique_ptr<rule>> rules_before_move;
		std::vector<std::unique_ptr<rule>> rules_after_move;

	public:
		ruleset(const std::vector<rule*>& rules_before, const std::vector<rule*>& rules_after);
		virtual ~ruleset() = default;

		void apply(board& b, const move& m, bool eval = false);

		virtual void init(board& b);
		virtual void reset(board& b);
		virtual void after_move(board& b, const move& m);
		std::vector<std::unique_ptr<abstract_action>>& get_actions();

		std::unique_ptr<abstract_action> next;
		virtual void set_next(abstract_action* action);
	};

} /* namespace chess */

#endif /* RULESET_HPP */
