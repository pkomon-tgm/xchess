/*
 * main.cpp
 *
 *  Created on: Apr 16, 2018
 *      Author: pkomon
 */

#include <exception>
#include <iostream>

#include "boost/asio/io_context.hpp"

#include "game_server.hpp"

int main(){

	try {
		boost::asio::io_context context;
		chess::game_server gs(context, 5555);
		//gs.start_accept();
		context.run();
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}
