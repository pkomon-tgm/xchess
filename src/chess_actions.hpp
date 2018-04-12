/*
 * chess_actions.hpp
 *
 *  Created on: Apr 12, 2018
 *      Author: pkomon
 */

#ifndef CHESS_ACTIONS_HPP_
#define CHESS_ACTIONS_HPP_

#include "ruleset.hpp"
#include "move.hpp"
#include "board.hpp"
#include "piece.hpp"

namespace chess {

	struct move_action : public abstract_action {
		move m;

		move_action(const move& m);

		virtual void execute(board& b);
		virtual void undo(board& b);

	};

	struct promotion_action : public abstract_action {
		std::unique_ptr<move_action> act;
		piece_type promote_to;

		//wraps other action, whether it was a hit or move, takes ownership
		promotion_action(std::unique_ptr<abstract_action>&& act, piece_type promote_to);

		virtual void execute(board& b);
		virtual void undo(board& b);
	};

	struct castling_action : public abstract_action {
		move_action king_action, tower_action;

		castling_action(const move& king_move, const move& tower_move);

		virtual void execute(board& b);
		virtual void undo(board& b);
	};

	struct hit_action : public move_action {
		piece hit;
		position hit_at;

		hit_action(const move& m, const piece& h);
		hit_action(const move& m, const piece& h, const position& hit_at);

		virtual void execute(board& b);
		virtual void undo(board& b);
	};

	//	struct compound_action : public abstract_action {
	//		std::vector<std::unique_ptr<abstract_action>> childs;
	//
	//		virtual void execute(board& b);
	//		virtual void undo(board& b);
	//	};


} //namespace chess

#endif /* CHESS_ACTIONS_HPP_ */
