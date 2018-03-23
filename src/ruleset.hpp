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

	class rule {
		std::set<piece_type> applies_to_types;

	public:
		rule(const std::set<piece_type>& applies_to_types);
		virtual ~rule() = default;

		bool apply(ruleset& rules, board& b, const move& m);

		virtual bool rule_callback(ruleset& rules, board& b, const move& m) = 0;

	private:
		bool applies_to(piece_type type) const;
	};

	using generic_rule_cb_function = std::function<bool(ruleset& rules, board& b, const move& m, const piece& p)>;

	class generic_rule : public rule {
		generic_rule_cb_function cb;

	public:
		generic_rule(const std::set<piece_type>& applies_to_types, const generic_rule_cb_function& cb);
		virtual bool rule_callback(ruleset& rules, board& b, const move& m) override;
	};


	class ruleset {
	protected:
		std::vector<std::unique_ptr<rule>> rules;

	public:
		ruleset(const std::vector<rule*>& rules);
		virtual ~ruleset() = default;

		void apply(board& b, const move& m);

		virtual void init(board& b);
		virtual void reset(board& b);
		virtual void after_move(board& b, const move& m);
	};

} /* namespace chess */

#endif /* RULESET_HPP */
