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
#include "board.hpp"
#include "move.hpp"
#include "position.hpp"

namespace chess {

	const std::set<piece_type> ALL_PIECE_TYPES{piece_type::PAWN, piece_type::ROCK, piece_type::KNIGHT,
										 piece_type::BISHOP, piece_type::QUEEN, piece_type::KING};

	chess_ruleset::chess_ruleset(const std::vector<rule*>& rules): ruleset{rules} {}


	chess_ruleset chess_rules{{
		new generic_rule{ALL_PIECE_TYPES, &non_empty_source_rule_cb},
		new generic_rule{ALL_PIECE_TYPES, &right_to_move_cb},
		new generic_rule{ALL_PIECE_TYPES, &different_source_target_rule_cb},
		new generic_rule{ALL_PIECE_TYPES, &discovered_attack_rule_cb},
		new generic_rule{ALL_PIECE_TYPES, &check_rule_cb},
		new generic_rule{ALL_PIECE_TYPES, &empty_target_rule_cb},
		new generic_rule{ALL_PIECE_TYPES, &move_in_bounds_cb},
		/*new generic_piece_rule{{piece_type::ROCK}, {{1, 0}, {0, 1}, {-1, 0}, {0, -1}}},
		new generic_piece_rule{{piece_type::KNIGHT}, {{1, 2}, {-1, 2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, -2}, {-1, -2}}, true, 1},
		new generic_piece_rule{{piece_type::BISHOP}, {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}}},
		new generic_piece_rule{{piece_type::QUEEN}, {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}, {-1, 0}, {0, -1}, {1, 0}, {0, 1}}},
		new generic_piece_rule{{piece_type::KING}, {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}, {-1, 0}, {0, -1}, {1, 0}, {0, 1}}, false, 1},*/
		new generic_piece_rule<piece_type::ROCK>(),
		new generic_piece_rule<piece_type::KNIGHT>(),
		new generic_piece_rule<piece_type::BISHOP>(),
		new generic_piece_rule<piece_type::QUEEN>(),
		new generic_piece_rule<piece_type::KING>(),
		//new generic_rule{piece_type::KING, &target_not_attacked_cb},
		//new pawn_piece_rule{},
		new generic_piece_rule<piece_type::PAWN>(),
		new generic_rule{ALL_PIECE_TYPES, &toggle_to_move_cb},
	}};

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

	piece_color chess_ruleset::get_to_move(){return to_move;}
	void chess_ruleset::toggle_to_move(){to_move =! to_move;}


	movement_spec::movement_spec(const std::vector<position>& relative_moves, unsigned int times, bool jump):
		relative_moves{relative_moves}, times{times}, jump{jump} {}

	std::unordered_map<piece_type, movement_spec> movement_db{
		{piece_type::ROCK, {{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}}},
		{piece_type::KNIGHT, {{{1, 2}, {-1, 2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, -2}, {-1, -2}}, 1, true}},
		{piece_type::BISHOP, {{{1, 1}, {-1, 1}, {1, -1}, {-1, -1}}}},
		{piece_type::QUEEN, {{{1, 1}, {-1, 1}, {1, -1}, {-1, -1}, {-1, 0}, {0, -1}, {1, 0}, {0, 1}}}},
		{piece_type::KING, {{{1, 1}, {-1, 1}, {1, -1}, {-1, -1}, {-1, 0}, {0, -1}, {1, 0}, {0, 1}}, 1, false}}
	};

	template<> std::vector<position> get_attacked_fields<PAWN>(board& b, const position& pos){
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

	template<> std::vector<position> get_possible_move_targets<PAWN>(board& b, const position& pos){
		std::vector<position> output;
		piece p = b.at(pos);
		int y_movement = p.get_color() == piece_color::WHITE ? 1 : -1;
		int baseline = p.get_color() == piece_color::WHITE ? 2 : 7;

		position possible_target = pos + position{0, y_movement};
		if (b.is_empty(possible_target))
			output.push_back(possible_target);
		if (pos.y == baseline){
			possible_target = pos + position{0, 2*y_movement};
			if (b.is_empty(possible_target))
				output.push_back(possible_target);
		}

		for (position attack : get_attacked_fields<PAWN>(b, pos))
			if (!b.is_empty(attack) && b.at(attack).get_color() != p.get_color())
				output.push_back(attack);

		return output;
	}



	void non_empty_source_rule_cb(ruleset& rules, board& b, const move& m, const piece& p){
		if(b.is_empty(m.source))
			throw invalid_move_error(m, "lol"); //TODO
	}

	void right_to_move_cb(ruleset& rules, board& b, const move& m, const piece& p){
		if(b.at(m.source).get_color() != static_cast<chess_ruleset&>(rules).get_to_move())
			throw invalid_move_error(m, "tried to move piece of wrong player");
	}


	void different_source_target_rule_cb(ruleset& rules, board& b, const move& m, const piece& p){
		if(m.source == m.target)
			throw invalid_move_error(m, "Source and target are the same!");
	}

	void discovered_attack_rule_cb(ruleset& rules, board& b, const move& m, const piece& p){

	}

	void check_rule_cb(ruleset& rules, board& b, const move& m, const piece& p){

	}

	void empty_target_rule_cb(ruleset& rules, board& b, const move& m, const piece& p){
		if(!b.is_empty(m.target) && b.at(m.target).get_color() == p.get_color())
			throw invalid_move_error(m, "Can't move on fields occupied with by piece");
	}

	void move_in_bounds_cb(ruleset& rules, board& b, const move& m, const piece& p){
		if(!b.is_in_bounds(m.source) || !b.is_in_bounds(m.target))
			throw invalid_move_error(m, "Source and/or target position of move not within board's bounds");
	}

	void toggle_to_move_cb(ruleset& rules, board& b, const move& m, const piece& p){
		static_cast<chess_ruleset&>(rules).toggle_to_move();
	}

	void target_not_attacked_cb(ruleset& rules, board& b, const move& m, const piece& p){

	}

	bool is_path_free(board& b, const position& source, const position& target, const position& offset){
		for(position current = source+offset; current != target; current+=offset){
			if(!b.is_empty(current))
				return false;
		}
		return true;
	}

	bool is_field_attacked_by(ruleset& rules, board& b, const position& pos, piece_color color){
		return true;
	}


}
