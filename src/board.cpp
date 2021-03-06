/*
 * board.cpp
 *
 *  Created on: Mar 5, 2018
 *      Author: pkomon
 */

#include "board.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

#include "piece.hpp"
#include "move.hpp"
#include "ruleset.hpp"

#define RED "\033[31m"
#define BLUE "\033[34m"
#define RESET "\033[0m"

namespace chess {

	board::board(unsigned short size): fields{}, size{size} {
		fields.reserve(size*size/2);
		reset_pieces();
	}

	unsigned short board::to_index(const position& pos){
		if(pos.x <= 0 && pos.x > size && pos.y <= 0 && pos.y > size) //maybe return -1 on error instead?
			throw std::range_error("position out of range");
		return pos.x-1 + (pos.y-1) * size;
	}

	void board::set_piece(const piece& p, const position& pos){fields.insert_or_assign(pos, p);}
	void board::remove_piece(const position& pos){fields.erase(pos);}
	void board::reset_pieces(){fields.clear();}

	piece& board::at(const position& pos) {return fields.find(pos)->second;}
	const piece& board::at(const position& pos) const {return fields.find(pos)->second;}

	bool board::is_empty(const position& pos) const {return fields.find(pos) == fields.end();}
	bool board::is_in_bounds(const position& pos) const {return pos.x > 0 && pos.x <= size && pos.y > 0 && pos.y <= size;};
	std::unordered_map<position, piece>& board::get_fields(){return fields;}


	std::string board::to_string(){
		std::stringstream ss;

		ss << " ";
		for(short x = 1; x <= size; ++x)
			ss << " " << position::to_char(x);
		ss << std::endl;

		for(short y = size; y >= 1; --y){
			ss << y << "|";
			for(short x = 1; x <= size; ++x){
				if(is_empty({x, y})){
					ss << " |";
				} else {
					ss << (at({x, y}).get_color() == piece_color::WHITE ? BLUE : RED) << at({x, y}).get_type() << RESET << "|";
				}
			}
			ss << y << std::endl;
		}
		ss << " ";
		for(short x = 1; x <= size; ++x)
			ss << " " << position::to_char(x);

		return ss.str();
	}


} /* namespace chess */
