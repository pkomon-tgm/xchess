/*
 * chess_actions.cpp
 *
 *  Created on: Apr 12, 2018
 *      Author: pkomon
 */

#include "chess_actions.hpp"

namespace chess {

	move_action::move_action(const move& m): m{m} {}

	void move_action::execute(board& b){
		b.set_piece(b.at(m.source), m.target);
		b.remove_piece(m.source);
	}

	void move_action::undo(board& b){
		b.set_piece(b.at(m.target), m.source);
		b.remove_piece(m.target);
	}

	promotion_action::promotion_action(std::unique_ptr<abstract_action>&& act, piece_type promote_to):
		act{static_cast<move_action*>(act.release())}, promote_to{promote_to} {}

	void promotion_action::execute(board& b){
		act->execute(b);
		b.set_piece({promote_to, b.at(act->m.target).get_color()}, act->m.target);
	}

	void promotion_action::undo(board& b){
		b.set_piece({PAWN, b.at(act->m.target).get_color()}, act->m.target);
		act->undo(b);
	}

	hit_action::hit_action(const move& m, const piece& hit): move_action{m}, hit{hit}, hit_at{m.target} {}
	hit_action::hit_action(const move& m, const piece& hit, const position& hit_at): move_action{m}, hit{hit}, hit_at{hit_at} {}

	void hit_action::execute(board& b){
		move_action::execute(b);
		if (hit_at != m.target)
			b.remove_piece(hit_at);
	}

	void hit_action::undo(board& b){
		move_action::undo(b);
		b.set_piece(hit, hit_at);
	}

	castling_action::castling_action(const move& king_move, const move& tower_move): king_action{king_move}, tower_action{tower_move} {}
	void castling_action::execute(board& b){
		king_action.execute(b);
		tower_action.execute(b);
	}

	void castling_action::undo(board& b){
		tower_action.undo(b);
		king_action.undo(b);
	}



} //namespace chess
