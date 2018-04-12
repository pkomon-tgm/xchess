/*
 * generic_rule.hpp
 *
 *  Created on: Apr 12, 2018
 *      Author: pkomon
 */

#ifndef GENERIC_RULE_HPP_
#define GENERIC_RULE_HPP_

#include <functional>
#include <set>

#include "rule.hpp"

namespace chess {

	using generic_rule_cb_function = std::function<bool(ruleset& rules, board& b, const move& m)>;

	class generic_rule : public rule {
		generic_rule_cb_function cb;

	public:
		generic_rule(const std::set<piece_type>& applies_to_types, const generic_rule_cb_function& cb);
		virtual bool rule_callback(ruleset& rules, board& b, const move& m) override;
	};



} //namespace chess

#endif /* GENERIC_RULE_HPP_ */
