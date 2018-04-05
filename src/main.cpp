/*
 * main.cpp
 *
 *  Created on: Mar 6, 2018
 *      Author: pkomon
 */

#include <iostream>

#include "game.hpp"
#include "board.hpp"
#include "chess_ruleset.hpp"

chess::position get_pos_from_cin(const std::string& prompt = ""){
	char x; int y;

	std::cout << prompt;
	std::cin >> x >> y;

	if(std::cin.fail()){
		std::cin.clear();
		std::cin.ignore(10000);
		return get_pos_from_cin(prompt);
	}
	return chess::position{x, y};
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
		//std::cout << "to move [0..WHITE, 1..BLACK]: " /*<< g.get_to_move()*/ << std::endl;
		auto src = get_pos_from_cin("src: ");
		auto dest = get_pos_from_cin("dest: ");
		std::cout << std::endl;
		try {
			g.make_move({src, dest});
		} catch(const chess::invalid_move_error& e) {
			std::cout << "move is not rule-compliant:" << std::endl;
			std::cout << e.what() << std::endl;
		}
		std::cout << g.get_board().to_string() << std::endl;
	}

	return 0;
}
