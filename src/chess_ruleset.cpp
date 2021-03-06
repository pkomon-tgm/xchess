/*
 * chess_ruleset.cpp
 *
 *  Created on: Mar 7, 2018
 *      Author: pkomon
 */

#include "chess_ruleset.hpp"

#include <cmath>
#include <map>
#include <algorithm>
#include <vector>

#include "util.hpp"

#include "ruleset.hpp"
#include "generic_rule.hpp"
#include "board.hpp"
#include "move.hpp"
#include "position.hpp"

#include "chess_actions.hpp"


#include <iostream>


namespace chess {

	promotion_move::promotion_move(const position& source, const position& target, piece_type promote_to):
			move{source, target}, promote_to{promote_to} {}

	const std::set<piece_type> ALL_PIECE_TYPES{piece_type::PAWN, piece_type::ROCK, piece_type::KNIGHT,
											   piece_type::BISHOP, piece_type::QUEEN, piece_type::KING};

	chess_ruleset::chess_ruleset(): ruleset{
		{
			new generic_rule{ALL_PIECE_TYPES, &right_to_move_cb},
			new generic_rule{ALL_PIECE_TYPES, &different_source_target_rule_cb},
			new generic_rule{ALL_PIECE_TYPES, &empty_target_rule_cb},
			new generic_rule{ALL_PIECE_TYPES, &move_in_bounds_cb},
			new generic_piece_rule<piece_type::ROCK>(),
			new generic_piece_rule<piece_type::KNIGHT>(),
			new generic_piece_rule<piece_type::BISHOP>(),
			new generic_piece_rule<piece_type::QUEEN>(),
			new castling_rule(),
			new generic_piece_rule<piece_type::KING>(),
			new generic_piece_rule<piece_type::PAWN>(),
			new generic_rule{{PAWN}, &pawn_promotion_cb},
		},
		{
			new generic_rule{ALL_PIECE_TYPES, &king_in_check_cb}
		}
	} {}

	/*chess_ruleset chess_rules{
		{
			new generic_rule{ALL_PIECE_TYPES, &right_to_move_cb},
			new generic_rule{ALL_PIECE_TYPES, &different_source_target_rule_cb},
			new generic_rule{ALL_PIECE_TYPES, &empty_target_rule_cb},
			new generic_rule{ALL_PIECE_TYPES, &move_in_bounds_cb},
			new generic_piece_rule<piece_type::ROCK>(),
			new generic_piece_rule<piece_type::KNIGHT>(),
			new generic_piece_rule<piece_type::BISHOP>(),
			new generic_piece_rule<piece_type::QUEEN>(),
			new castling_rule(),
			new generic_piece_rule<piece_type::KING>(),
			new generic_piece_rule<piece_type::PAWN>(),
			new generic_rule{{PAWN}, &pawn_promotion_cb},
		},
		{
			new generic_rule{ALL_PIECE_TYPES, &king_in_check_cb}
		}
	};*/

	void chess_ruleset::init(board& b){
		for(auto i = 1; i <= 8; ++i)
			b.set_piece({piece_type::PAWN, piece_color::WHITE}, {position::to_char(i), 2});

		b.set_piece({piece_type::ROCK, piece_color::WHITE}, {'A', 1});
		b.set_piece({piece_type::ROCK, piece_color::WHITE}, {'H', 1});
		b.set_piece({piece_type::KNIGHT, piece_color::WHITE}, {'B', 1});
		b.set_piece({piece_type::KNIGHT, piece_color::WHITE}, {'G', 1});
		b.set_piece({piece_type::BISHOP, piece_color::WHITE}, {'C', 1});
		b.set_piece({piece_type::BISHOP, piece_color::WHITE}, {'F', 1});
		b.set_piece({piece_type::QUEEN, piece_color::WHITE}, {'D', 1});
		b.set_piece({piece_type::KING, piece_color::WHITE}, {'E', 1});

		for(auto i = 1; i <= 8; ++i)
			b.set_piece({piece_type::PAWN, piece_color::BLACK}, {position::to_char(i), 7});

		b.set_piece({piece_type::ROCK, piece_color::BLACK}, {'A', 8});
		b.set_piece({piece_type::ROCK, piece_color::BLACK}, {'H', 8});
		b.set_piece({piece_type::KNIGHT, piece_color::BLACK}, {'B', 8});
		b.set_piece({piece_type::KNIGHT, piece_color::BLACK}, {'G', 8});
		b.set_piece({piece_type::BISHOP, piece_color::BLACK}, {'C', 8});
		b.set_piece({piece_type::BISHOP, piece_color::BLACK}, {'F', 8});
		b.set_piece({piece_type::QUEEN, piece_color::BLACK}, {'D', 8});
		b.set_piece({piece_type::KING, piece_color::BLACK}, {'E', 8});
	}

	checkmate::checkmate(piece_color winner): winner{winner} {}

	void chess_ruleset::after_move(board& b, const move& m){
		toggle_to_move();
		if(is_checkmate(*this, b, to_move)){
			throw checkmate(!to_move);
		}
	}

	piece_color chess_ruleset::get_to_move(){return to_move;}
	void chess_ruleset::toggle_to_move(){to_move = !to_move;}


	movement_spec::movement_spec(const std::vector<position>& relative_moves, unsigned int times, bool jump):
		relative_moves{relative_moves}, times{times}, jump{jump} {}

	std::unordered_map<piece_type, movement_spec> movement_db{
		{piece_type::ROCK, {{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}}},
		{piece_type::KNIGHT, {{{1, 2}, {-1, 2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, -2}, {-1, -2}}, 1, true}},
		{piece_type::BISHOP, {{{1, 1}, {-1, 1}, {1, -1}, {-1, -1}}}},
		{piece_type::QUEEN, {{{1, 1}, {-1, 1}, {1, -1}, {-1, -1}, {-1, 0}, {0, -1}, {1, 0}, {0, 1}}}},
		{piece_type::KING, {{{1, 1}, {-1, 1}, {1, -1}, {-1, -1}, {-1, 0}, {0, -1}, {1, 0}, {0, 1}}, 1, false}}
	};

	//TODO find smth more elegant
	std::map<piece_type, std::pair<get_possible_move_targets_cb, get_attacked_fields_cb>> movement_attack_cb_map{
		{PAWN, {&get_possible_move_targets<PAWN>, &get_attacked_fields<PAWN>}},
		{BISHOP, {&get_possible_move_targets<BISHOP>, &get_attacked_fields<BISHOP>}},
		{ROCK, {&get_possible_move_targets<ROCK>, &get_attacked_fields<ROCK>}},
		{KNIGHT, {&get_possible_move_targets<KNIGHT>, &get_attacked_fields<KNIGHT>}},
		{QUEEN, {&get_possible_move_targets<QUEEN>, &get_attacked_fields<QUEEN>}},
		{KING, {&get_possible_move_targets<KING>, &get_attacked_fields<KING>}}
	};

	std::vector<position> get_attacked_fields(chess_ruleset& rules, board& b, const position& pos){
		return movement_attack_cb_map[b.at(pos).get_type()].second(rules, b, pos);
	}

	std::vector<position> get_possible_move_targets(chess_ruleset& rules, board& b, const position& pos){
		return movement_attack_cb_map[b.at(pos).get_type()].first(rules, b, pos, false);
	}

	std::vector<position> get_field_attackers(chess_ruleset& rules, board& b, const position& pos, piece_color color){
		std::vector<position> out;
		for(auto& pos_piece: b.get_fields()){
			if(pos_piece.second.get_color() == color && util::vector_contains(get_attacked_fields(rules, b, pos_piece.first), pos)){
				out.push_back(pos_piece.first);
			}
		}
		return out;
	}

	bool is_field_attacked_by(chess_ruleset& rules, board& b, const position& pos, piece_color color){
		//more optimized
		/*for(auto& pos_piece: b.get_fields()){
			if(pos_piece.second.get_color() == color && util::vector_contains(get_attacked_fields(rules, b, pos_piece.first), pos)){
				return true;
			}
		}
		return false;*/
		return !get_field_attackers(rules, b, pos, color).empty();
	}


	template<> std::vector<position> get_attacked_fields<PAWN>(chess_ruleset& rules, board& b, const position& pos){
		std::vector<position> output;
		piece_color color = b.at(pos).get_color();
		int y_movement = color == piece_color::WHITE ? 1 : -1;
		position attack_left = pos + position{-1, y_movement};
		position attack_right = pos + position{1, y_movement};
		if (b.is_in_bounds(attack_left))
			output.push_back(attack_left);
		if (b.is_in_bounds(attack_right))
			output.push_back(attack_right);
		return output;
	}

	template<> std::vector<position> get_possible_move_targets<PAWN>(chess_ruleset& rules, board& b, const position& pos, bool include_own_pieces){
		std::vector<position> output;
		piece p = b.at(pos);
		int y_movement = p.get_color() == piece_color::WHITE ? 1 : -1;
		int pawnline = get_baseline(p.get_color()) + y_movement;

		position possible_target = pos + position{0, y_movement};
		if (b.is_empty(possible_target))
			output.push_back(possible_target);
		if (pos.y == pawnline){
			possible_target = pos + position{0, 2*y_movement};
			if (b.is_empty(possible_target))
				output.push_back(possible_target);
		}

		for (const position& attack : get_attacked_fields(rules, b, pos)){
			if (!b.is_empty(attack) && b.at(attack).get_color() != p.get_color())
				output.push_back(attack);
			else if (b.is_empty(attack) && pos.y == pawnline + 3*y_movement
			      && !b.is_empty({attack.x, attack.y - y_movement})
				  && b.at({attack.x, attack.y - y_movement}) == piece{PAWN, !p.get_color()}){
				if (move_action* last_action = dynamic_cast<move_action*>(rules.get_actions().back().get())){
					const move& m = last_action->m;
					if (m.source == position{attack.x, attack.y-y_movement*-1} && m.target == position{attack.x, attack.y+y_movement*-1})
						// en passant!
						output.push_back(attack);
				}
			}
		}

		return output;
	}

	castling_rule::castling_rule(): rule{{piece_type::KING}} {}
	bool castling_rule::rule_callback(ruleset& rules, board& b, const move& m){ //maybe prettify
		piece_color my_color = b.at(m.source).get_color();
		int baseline = my_color == piece_color::WHITE ? 1 : 8;

		if(m.source.y == baseline && m.target.y == baseline
				&& m.source.x == position::to_index('E') && std::abs(m.source.x - m.target.x) == 2){
			int dir = m.source.x - m.target.x == 2 ? -1 : 1;
			position original_tower_pos = position{dir == -1 ? 1 : 8, baseline};
			if(!b.is_empty(original_tower_pos) && b.at(original_tower_pos).get_color() == my_color
			 && b.at(original_tower_pos).get_type() == ROCK
			 && !has_moved(rules, original_tower_pos) && !has_moved(rules, m.source)){
				position offset = {dir, 0};
				if(is_path_free(b, m.source, original_tower_pos, offset)){
					for(position current = m.source; current != m.target; current+=offset){
						if(is_field_attacked_by(static_cast<chess_ruleset&>(rules), b, current, !my_color))
							return false;
					}
					rules.set_next(new castling_action{m, {original_tower_pos, m.target-offset}});
					return true;
				}
			}
		}

		return false;
	}

	bool right_to_move_cb(ruleset& rules, board& b, const move& m){
		if(b.at(m.source).get_color() != static_cast<chess_ruleset&>(rules).get_to_move())
			throw invalid_move_error(m, "tried to move piece of wrong player");
		return false;
	}

	bool different_source_target_rule_cb(ruleset& rules, board& b, const move& m){
		if(m.source == m.target)
			throw invalid_move_error(m, "Source and target are the same!");
		return false;
	}

	bool empty_target_rule_cb(ruleset& rules, board& b, const move& m){
		if(!b.is_empty(m.target) && b.at(m.target).get_color() == b.at(m.source).get_color())
			throw invalid_move_error(m, "Can't move on fields occupied by own piece");
		return false;
	}

	bool move_in_bounds_cb(ruleset& rules, board& b, const move& m){
		if(!b.is_in_bounds(m.source) || !b.is_in_bounds(m.target))
			throw invalid_move_error(m, "Source and/or target position of move not within board's bounds");
		return false;
	}

	bool pawn_promotion_cb(ruleset& rules, board& b, const move& m){
		int enemy_baseline = b.at(m.source).get_color() == piece_color::WHITE ? 8 : 1;
		if(m.target.y == enemy_baseline){
			if(const promotion_move* pr_move = dynamic_cast<const promotion_move*>(&m))
				rules.set_next(new promotion_action{std::move(rules.next), pr_move->promote_to}); //wrap last move set
			else
				throw invalid_move_error{m, "need to pass promotion_move when moving a pawn to enemy's baseline!"};
		}
		return false;
	}

	bool king_in_check_cb(ruleset& rules, board& b, const move& m){
		piece_color own_color = b.at(m.target).get_color();
		for(auto& pos_piece : b.get_fields()){
			if (pos_piece.second == piece{KING, own_color}){
				if(is_field_attacked_by(static_cast<chess_ruleset&>(rules), b, pos_piece.first, !own_color)){
					throw invalid_move_error(m, "own king would be in check at the end of this move");
				}
			}
		}
		return false;
	}

	int get_baseline(piece_color c){
		return c == piece_color::WHITE ? 1 : 8;
	}

	bool is_path_free(board& b, const position& source, const position& target, const position& offset){
		for(position current = source+offset; current != target; current+=offset){
			if(!b.is_empty(current))
				return false;
		}
		return true;
	}

	bool has_moved(ruleset& rules, const position& pos){
		for(auto& action : rules.get_actions()){
			//maybe rather add smth. like std::vector<position> involves(); to action...?
			move_action* actual_move_action;

			if(const promotion_action* pr_action = dynamic_cast<const promotion_action*>(action.get())){
				actual_move_action = pr_action->act.get();
			} else
				actual_move_action = static_cast<move_action*>(action.get());

			if(actual_move_action->m.source == pos || actual_move_action->m.target == pos)
				return true;

			if(const hit_action* h_action = dynamic_cast<const hit_action*>(action.get()))
				if(h_action->hit_at == pos)
					return true;

		}
		return false;
	}


	position get_piece_position(board& b, piece_color color, piece_type type){
		for(auto& pos_piece : b.get_fields())
			if(pos_piece.second.get_color() == color && pos_piece.second.get_type() == KING)
				return pos_piece.first;
		//todo: throw piece_not_found_exception{};
		return {-1, -1};
	}

	bool is_checkmate(ruleset& rules, board& b, piece_color color){

		auto get_attack_vector = [&b](position start, position vec)->std::vector<position>{
			std::vector<position> out;
			position offset{vec.x > 0 ? 1 : (vec.x < 0 ? -1 : 0), vec.y > 0 ? 1 : (vec.y < 0 ? -1 : 0)};
			position target = start + vec;
			for (start+=offset; start!=target; start+=offset)
				out.push_back(start);
			return out;
		};

		//todo
		//get king pos
		const position& king_position = get_piece_position(b, color, KING);
		if (!is_field_attacked_by(static_cast<chess_ruleset&>(rules), b, king_position, !color)){
			return false;
		}

		//check if king can move out
		for(auto& king_move_target : get_possible_move_targets(static_cast<chess_ruleset&>(rules), b, king_position)){
			if(!is_field_attacked_by(static_cast<chess_ruleset&>(rules), b, king_move_target, !color))
				return false;
		}
		//get position of pieces attacking
		auto attacking_positions = get_field_attackers(static_cast<chess_ruleset&>(rules), b, king_position, !color);
		for (auto& attacking_position : attacking_positions){
			auto hit_candidates = get_field_attackers(static_cast<chess_ruleset&>(rules), b, attacking_position, color);
			//evaluate moves hit_candidate -> attacking_position - can attacking piece be hit?
			for (auto& hit_candidate : hit_candidates){
				try {
					rules.apply(b, move{hit_candidate, attacking_position}, true);
					return false;
				} catch (invalid_move_error& e) {
					//std::cout << "evaluate failed: " << e.what() << std::endl;
				}
			}

			//test if own piece into attack vector, effectively blocking the attack
			piece_type attacking_piece_type = b.at(attacking_position).get_type();

			if (attacking_piece_type == ROCK || attacking_piece_type == BISHOP || attacking_piece_type == QUEEN){
				auto attack_vector = get_attack_vector(attacking_position, king_position - attacking_position);
				for (auto& attack_vector_position : attack_vector){
					for (auto& pos_piece : b.get_fields()){
						if (pos_piece.second.get_color() == color
						  && util::vector_contains(get_possible_move_targets(static_cast<chess_ruleset&>(rules), b, pos_piece.first), attack_vector_position)){
							try {
								rules.apply(b, move{pos_piece.first, attack_vector_position}, true);
								return false;
							} catch (invalid_move_error& e) {
								//std::cout << "evaluate failed: " << e.what() << std::endl;
							}
						}
					}
				}
			}
		}
		return true;
	}
}
