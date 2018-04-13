/*
 * main.cpp
 *
 *  Created on: Mar 6, 2018
 *      Author: pkomon
 */

#include <iostream>
#include "boost/algorithm/string.hpp"

#include "game.hpp"
#include "board.hpp"
#include "chess_ruleset.hpp"


chess::position to_position(const std::string& s){
	char col = s[0];
	int row = std::stoi(s.substr(1));
	return chess::position{col, row};
}


chess::move* get_move_from_cin(const std::string& prompt =""){
	static std::map<std::string, chess::piece_type> str_to_piece_type{
		{"rock", chess::piece_type::ROCK},
		{"knight", chess::piece_type::KNIGHT},
		{"bishop", chess::piece_type::BISHOP},
		{"queen", chess::piece_type::QUEEN},
	};

	do {
		std::string in;
		std::cout << prompt;
		std::getline(std::cin, in);
		std::vector<std::string> in_parts;
		boost::split(in_parts, in, boost::is_space());

		try {
			if (in_parts.size() == 2){
				return new chess::move{to_position(in_parts[0]), to_position(in_parts[1])};
			} else if (in_parts.size() == 3){
				if (str_to_piece_type.find(in_parts[2]) != str_to_piece_type.end())
					return new chess::promotion_move{to_position(in_parts[0]), to_position(in_parts[1]), str_to_piece_type[in_parts[2]]};
				else
					std::cout << "promotion_type not found; allowed values are 'rock', 'knight', 'bishop', 'queen'" << std::endl;
			} else {
				std::cout << "wrong format for move: need to be 'POS POS [promotion_type]', e.g. E2 E4 or B7 B8 queen" << std::endl;
			}
		} catch (const std::invalid_argument& e){
			std::cout << "could not parse token to position - positions need to be in format [char][int], e.g. E2 or F7" << std::endl;
		}
	} while(true);
}

int main(int argc, char **argv) {

	/*
	 * interface spec:
	 *
	 * game.move({{'E', 2}, {'E', 4}});
	 * game.move({{'E', 4}, {'E', 5}}); -> throws exception, color to move is black
	 * game.move({{'E', 7}, {'E', 5}});
	 * game.move({{'A', 1}, {'A', 5}}); -> throws exception, move impossible (due to other piece in the way)
	 * ...
	 *
	 */

	chess::game g{chess::board{8}, chess::chess_rules};

	std::cout << "chess testing" << std::endl;
	std::cout << g.get_board().to_string() << std::endl;

	while(true){
		std::unique_ptr<chess::move> m(
				get_move_from_cin(std::string(chess::chess_rules.get_to_move() == chess::piece_color::WHITE ? "blue (white) to move": "red (black) to move") + std::string(" your move: ")));
		std::cout << std::endl;
		try {
			g.make_move(*m);
		} catch(const chess::invalid_move_error& e) {
			std::cout << "move is not rule-compliant:" << std::endl;
			std::cout << e.what() << std::endl;
		} catch(const chess::checkmate& e) {
			std::cout << g.get_board().to_string() << std::endl;
			std::cout << "checkmate! " << (e.winner == chess::piece_color::WHITE ? "blue (white) " : "red (black) ") << "wins. Congrats!" << std::endl;
			break;
		}
		std::cout << g.get_board().to_string() << std::endl;
	}

	return 0;
}
