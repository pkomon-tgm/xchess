/*
 * chess_ruleset.hpp
 *
 *  Created on: Mar 7, 2018
 *      Author: pkomon
 */

#ifndef CHESS_RULESET_HPP
#define CHESS_RULESET_HPP

#include "ruleset.hpp"

#include <limits>
#include <vector>
#include <unordered_map>

#include "piece.hpp"
#include "position.hpp"
#include "board.hpp"
#include "util.hpp"

namespace chess {

	//struct piece_rule;

	struct action : public abstract_action {
		move m;

		action(const move& m);
		virtual ~action() = default;

		virtual void execute(board& b);
		virtual void undo(board& b);

	};

	struct hit_action : public action {
		piece hit;

		hit_action(const move& m, const piece& h);

		virtual void undo(board& b);

	};

//	struct compound_action : public action {
//
//
//		virtual void execute(board& b);
//		virtual void undo(board& b);
//	};

	class chess_ruleset : public ruleset {
		piece_color to_move = piece_color::WHITE;

	public:
		using ruleset::ruleset;

		virtual void init(board& b) override;
		virtual void after_move(board& b, const move& m) override;

		piece_color get_to_move();
		void toggle_to_move();

	};

	extern chess_ruleset chess_rules;


	//HELPER FUNCTIONS

	bool is_path_free(board& b, const position& source, const position& target, const position& offset);
	bool is_field_attacked_by(board& b, const position& pos, piece_color color);


	//GENERAL PIECE MOVEMENT

	struct movement_spec{
		std::vector<position> relative_moves;
		unsigned int times;
		bool jump;

		movement_spec(const std::vector<position>& relative_moves,
					  unsigned int times = std::numeric_limits<int>::infinity(), bool jump = false);
	};

	extern std::unordered_map<piece_type, movement_spec> movement_db;

	template<piece_type T> std::vector<position> get_possible_move_targets(board& b, const position& pos){
		movement_spec& spec = movement_db.at(T);
		std::vector<position> output;
		piece& piece = b.at(pos);

		for(position possible_move : spec.relative_moves) {
			position possible_target = pos + possible_move;
			for(unsigned int factor = 1; (factor <= spec.times || spec.times == std::numeric_limits<unsigned int>::infinity()) && b.is_in_bounds(possible_target); ++factor){ //TODO

				if(!b.is_empty(possible_target) && b.at(possible_target).get_color() == piece.get_color())
					break;

				if(spec.jump){
					output.push_back(possible_target);
					continue;
				}
				if(is_path_free(b, pos, possible_target, possible_move) && (b.is_empty(possible_target) || b.at(possible_target).get_color() != piece.get_color())){
					output.push_back(possible_target);
				} else
					break;

				possible_target += possible_move;
			}
		}

		return output;
	}

	template<piece_type T> std::vector<position> get_attacked_fields(board& b, const position& pos){
		return get_possible_move_targets<T>(b, pos);
	}

	template<> std::vector<position> get_attacked_fields<PAWN>(board& b, const position& pos);
	template<> std::vector<position> get_possible_move_targets<PAWN>(board& b, const position& pos);

	extern std::map<piece_type, std::vector<position> (*)(board& b, const position& pos)> get_attacked_fields_cb_map;

	std::vector<position> get_attacked_fields(board& b, const position& pos);

	//SPECIAL RULES

	template <piece_type T>
	struct generic_piece_rule : public rule {
		generic_piece_rule(): rule{{T}} {}
		virtual bool rule_callback(ruleset& rules, board& b, const move& m){
			if(!util::vector_contains(get_possible_move_targets<T>(b, m.source), m.target))
				throw invalid_move_error(m, "Generic move check failed: that piece can't move like this.");

			if(b.is_empty(m.target))
				static_cast<chess_ruleset&>(rules).next = new action(m);
			else
				static_cast<chess_ruleset&>(rules).next = new hit_action(m, b.at(m.target));

			return false;
		}
	};

	struct castling_rule : public rule {;
		castling_rule();
		virtual bool rule_callback(ruleset& rules, board& b, const move& m);
	};

	//FUNCTIONS FOR GENERIC RULES

	bool non_empty_source_rule_cb(ruleset& rules, board& b, const move& m);
	bool right_to_move_cb(ruleset& rules, board& b, const move& m);
	bool different_source_target_rule_cb(ruleset& rules, board& b, const move& m);

	/*move must not result in check to own king*/
	bool discovered_attack_rule_cb(ruleset& rules, board& b, const move& m);

	/*when own king is in check, move must do one of the following:
	 * - capture checking piece
	 * - move king to a field where it is not in check
	 * - blocking the check
	 */
	bool check_rule_cb(ruleset& rules, board& b, const move& m);

	/*move piece only to field if there isn't already a piece
	  of the same color or the enemy king*/
	bool empty_target_rule_cb(ruleset& rules, board& b, const move& m);

	/*check is source and target positions of move are actually within board bounds*/
	bool move_in_bounds_cb(ruleset& rules, board& b, const move& m);

	/*generic move rule, only pawn is not supported by this*/
	bool generic_move_rule_cb(ruleset& rules, board& b, const move& m);

	/*toggles to_move flag*/
	bool king_in_check_cb(ruleset& rules, board& b, const move& m);

	bool target_not_attacked_cb(ruleset& rules, board& b, const move& m);

}

#endif /* CHESS_RULESET_HPP */
