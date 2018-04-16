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
#include "rule.hpp"

#include "chess_actions.hpp"

namespace chess {

	//struct piece_rule;

	struct promotion_move : public move {
		piece_type promote_to;

		promotion_move(const position& source, const position& target, piece_type promote_to);
	};


	class chess_ruleset : public ruleset {
		piece_color to_move = piece_color::WHITE;

	public:
		chess_ruleset();

		virtual void init(board& b) override;
		virtual void after_move(board& b, const move& m) override;

		piece_color get_to_move();
		void toggle_to_move();
	};


	struct checkmate : public std::exception {
		piece_color winner;
		checkmate(piece_color winner);
	};

	//HELPER FUNCTIONS

	int get_baseline(piece_color c);
	position get_piece_position(board& b, piece_color color, piece_type type);
	bool is_path_free(board& b, const position& source, const position& target, const position& offset);
	bool is_field_attacked_by(chess_ruleset& rules, board& b, const position& pos, piece_color color);
	bool has_moved(ruleset& rules, const position& pos); //checks if position was involved in any movement as source or target (also check en passant?)
	bool is_checkmate(ruleset& rules, board& b, piece_color color);


	//GENERAL PIECE MOVEMENT

	struct movement_spec{
		std::vector<position> relative_moves;
		unsigned int times;
		bool jump;

		movement_spec(const std::vector<position>& relative_moves,
					  unsigned int times = std::numeric_limits<int>::infinity(), bool jump = false);
	};

	extern std::unordered_map<piece_type, movement_spec> movement_db;

	template<piece_type T> std::vector<position> get_possible_move_targets(chess_ruleset& rules, board& b, const position& pos, bool include_own_pieces=false){
		movement_spec& spec = movement_db.at(T);
		std::vector<position> output;
		piece& piece = b.at(pos);

		for(position possible_move : spec.relative_moves) {
			position possible_target = pos + possible_move;
			for(unsigned int factor = 1; (factor <= spec.times || spec.times == std::numeric_limits<unsigned int>::infinity()) && b.is_in_bounds(possible_target); ++factor){ //TODO

				if((!b.is_empty(possible_target) && b.at(possible_target).get_color() == piece.get_color()) && !include_own_pieces)
					break;

				if(spec.jump){
					output.push_back(possible_target);
					continue;
				}
				if(is_path_free(b, pos, possible_target, possible_move) && ((b.is_empty(possible_target) || b.at(possible_target).get_color() != piece.get_color()) || include_own_pieces)){
					output.push_back(possible_target);
				} else
					break;

				possible_target += possible_move;
			}
		}

		return output;
	}

	template<piece_type T> std::vector<position> get_attacked_fields(chess_ruleset& rules, board& b, const position& pos){
		return get_possible_move_targets<T>(rules, b, pos, true);
	}

	template<> std::vector<position> get_attacked_fields<PAWN>(chess_ruleset& rules, board& b, const position& pos);
	template<> std::vector<position> get_possible_move_targets<PAWN>(chess_ruleset& rules, board& b, const position& pos, bool include_own_pieces);

	using get_attacked_fields_cb = std::vector<position> (*)(chess_ruleset& rules, board& b, const position& pos);
	using get_possible_move_targets_cb = std::vector<position> (*)(chess_ruleset& rules, board& b, const position& pos, bool);

	extern std::map<piece_type, std::pair<get_possible_move_targets_cb, get_attacked_fields_cb>> movement_attack_cb_map;
	//extern std::map<piece_type, get_possible_move_targets_cb> get_possible_move_targets_cb_map;

	std::vector<position> get_possible_move_targets(chess_ruleset& rules, board& b, const position& pos);
	std::vector<position> get_attacked_fields(chess_ruleset& rules, board& b, const position& pos);


	//SPECIAL RULES

	template <piece_type T>
	struct generic_piece_rule : public rule {
		generic_piece_rule(): rule{{T}} {}
		virtual bool rule_callback(ruleset& rules, board& b, const move& m){
			if(!util::vector_contains(get_possible_move_targets<T>(static_cast<chess_ruleset&>(rules), b, m.source), m.target))
				throw invalid_move_error(m, "Generic move check failed: that piece can't move like this.");

			if(b.is_empty(m.target) && b.at(m.source).get_type() == PAWN && m.source.x != m.target.x)
				// means hit by pawn on empty field -> en passant
				rules.set_next(new hit_action(m, b.at({m.target.x, m.source.y}), {m.target.x, m.source.y}));
			else if (b.is_empty(m.target))
				rules.set_next(new move_action(m));
			else
				rules.set_next(new hit_action(m, b.at(m.target)));

			return false;
		}
	};

	struct castling_rule : public rule {;
		castling_rule();
		virtual bool rule_callback(ruleset& rules, board& b, const move& m);
	};

	//FUNCTIONS FOR GENERIC RULES

	bool right_to_move_cb(ruleset& rules, board& b, const move& m);
	bool different_source_target_rule_cb(ruleset& rules, board& b, const move& m);
	bool empty_target_rule_cb(ruleset& rules, board& b, const move& m);

	/*check is source and target positions of move are actually within board bounds*/
	bool move_in_bounds_cb(ruleset& rules, board& b, const move& m);

	bool pawn_promotion_cb(ruleset& rules, board& b, const move& m);

	bool king_in_check_cb(ruleset& rules, board& b, const move& m);

}

#endif /* CHESS_RULESET_HPP */
