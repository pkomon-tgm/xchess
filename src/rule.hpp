/*
 * rule.hpp
 *
 *  Created on: Apr 12, 2018
 *      Author: pkomon
 */

#ifndef RULE_HPP_
#define RULE_HPP_

#include <set>

#include "piece.hpp"

namespace chess {

	//fw
	class ruleset;
	class board;
	class move;

	class rule {
			std::set<piece_type> applies_to_types;

		public:
			rule(const std::set<piece_type>& applies_to_types);
			virtual ~rule() = default;

			bool apply(ruleset& rules, board& b, const move& m, piece_type type);

			virtual bool rule_callback(ruleset& rules, board& b, const move& m) = 0;

		private:
			bool applies_to(piece_type type) const;
	};
} //namespace chess

#endif /* RULE_HPP_ */
