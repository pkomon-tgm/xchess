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

//#include "board.hpp"
#include "move.hpp"

#include <memory>
#include <functional>

namespace chess {

	class board; //fw
	class ruleset;

	struct abstract_action{
		virtual ~abstract_action() = default;
		virtual void execute(board& b) = 0;
		virtual void undo(board& b) = 0;
	};

	class rule {
		std::set<piece_type> applies_to_types;

	public:
		rule(const std::set<piece_type>& applies_to_types);
		virtual ~rule() = default;

		bool apply(ruleset& rules, board& b, const move& m);
		bool apply(ruleset& rules, board& b, const move& m, piece_type type);

		virtual bool rule_callback(ruleset& rules, board& b, const move& m) = 0;

	private:
		bool applies_to(piece_type type) const;
	};

	using generic_rule_cb_function = std::function<bool(ruleset& rules, board& b, const move& m)>;

	class generic_rule : public rule {
		generic_rule_cb_function cb;

	public:
		generic_rule(const std::set<piece_type>& applies_to_types, const generic_rule_cb_function& cb);
		virtual bool rule_callback(ruleset& rules, board& b, const move& m) override;
	};


	class ruleset {

	protected:
		std::vector<std::unique_ptr<abstract_action>> actions;
		std::vector<std::unique_ptr<rule>> rules_before_move;
		std::vector<std::unique_ptr<rule>> rules_after_move;

	public:
		ruleset(const std::vector<rule*>& rules_before, const std::vector<rule*>& rules_after);
		virtual ~ruleset() = default;

		void apply(board& b, const move& m);

		virtual void init(board& b);
		virtual void reset(board& b);
		virtual void after_move(board& b, const move& m);
		std::vector<std::unique_ptr<abstract_action>>& get_actions();

		abstract_action* next;
	};

} /* namespace chess */

#endif /* RULESET_HPP */
