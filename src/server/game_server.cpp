/*
 * game_server.cpp
 *
 *  Created on: Apr 16, 2018
 *      Author: pkomon
 */

#include "game_server.hpp"

#include <memory>
#include <iostream>
#include <string>

#include "boost/bind.hpp"

#include "boost/asio/ip/tcp.hpp"
#include "boost/asio/placeholders.hpp"
#include "boost/asio/write.hpp"

#include "boost/algorithm/string/trim.hpp"
#include "boost/algorithm/string/split.hpp"

#include "challenge.hpp"

namespace chess {
//REMOVE
chess::position to_position(const std::string& s){
	char col = s[0];
	int row = std::stoi(s.substr(1));
	return chess::position{col, row};
}
//end
	const std::string game_server::GREETING = "HELLO! tell me ur name: ";
	const char game_server::COMMAND_PREFIX = '/';

	game_server::game_server(boost::asio::io_context& context, unsigned int port): acceptor{context},
		callbacks{},
		default_callback(new generic_server_callback([](game_server& gs, player& con, const std::vector<std::string>& args){
			for (std::shared_ptr<player> other : gs.get_players()) {
				if (*other != con){
					other->get_client_connection().get_socket().send(boost::asio::buffer(con.get_name()+": "+args[0]+"\n"));
				}
			}
		})),
		chal_mgr{}
	{
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
		acceptor.open(endpoint.protocol());
		acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		acceptor.bind(endpoint);
		acceptor.listen(10);
		start_accept();

		callbacks.reserve(5);
		callbacks.emplace_back("lol", new generic_server_callback([](game_server& gs, player& con, const std::vector<std::string>& args){
			std::cout << con.get_name() << " typed /lol and though he'd get a funny joke!" << std::endl;
		}));
		callbacks.emplace_back("invite", new challenge_invite_callback(chal_mgr));
		callbacks.emplace_back("accept", new challenge_accept_callback(chal_mgr));
		callbacks.emplace_back("showchallenges", new challenge_show_callback(chal_mgr));
		callbacks.emplace_back("showsessions", new session_show_callback(chal_mgr));

		callbacks.emplace_back("activate", new generic_server_callback([](game_server& gs, player& con, const std::vector<std::string>& args){
			if(args.size() < 2)
				throw std::runtime_error{"too few arguments"};
			int session_index = std::stoi(args[1]);
			auto sessions = gs.get_challenge_mgr().get_sessions_with(con);
			if (sessions.size() == 0)
				throw std::runtime_error{"no sessions for this player!"};
			if (session_index < sessions.size()){
				con.set_active_session(*sessions[session_index]);
			}

		}));

		callbacks.emplace_back("show", new generic_server_callback([](game_server& gs, player& con, const std::vector<std::string>& args){
			if(con.get_active_session() == nullptr)
				throw std::runtime_error{"no session active, need to exec /active [index]"};
			con.get_client_connection().get_socket().send(boost::asio::buffer(con.get_active_session()->get_model().get_board().to_string()));

		}));

		callbacks.emplace_back("move", new generic_server_callback([](game_server& gs, player& con, const std::vector<std::string>& args){
			if(args.size() < 3)
				throw std::runtime_error{"too few arguments"};
			if(con.get_active_session() == nullptr)
				throw std::runtime_error{"no session active, need to exec /active [index]"};

			auto& active_session = *con.get_active_session();
			active_session.get_model().make_move(move{to_position(args[1]), to_position(args[2])});
		}));


	}

	std::string format_msg(boost::asio::mutable_buffer& str_buf, std::size_t size){
		auto read = static_cast<char*>(str_buf.data());
		std::string input_str(read); //todo: fix error on to large strings
		input_str.resize(size); //properly format string (null terminator)
		return boost::trim_copy(input_str);
	}

	void game_server::start_accept(){
		std::shared_ptr<boost::asio::ip::tcp::socket> socket(new boost::asio::ip::tcp::socket(acceptor.get_executor().context()));

		//acceptor.async_accept(*socket.get(), boost::bind(&game_server::accept_handler, this, socket, boost::asio::placeholders::error));
		acceptor.async_accept(*socket.get(), [this, socket](const boost::system::error_code& e){
			if (!e) {
				std::cout << "client connected - woooohooooooo" << std::endl;

				boost::asio::async_write(*socket, boost::asio::buffer(GREETING, GREETING.size()), //assuming a char is 8 byte
					[this, socket](const boost::system::error_code& error, std::size_t bytes_transferred){
						if (!error) {
							std::shared_ptr<boost::asio::mutable_buffer> input_buf(new boost::asio::mutable_buffer(new std::string(""), 50));
							socket->async_read_some(*input_buf, [this, socket, input_buf](const boost::system::error_code& error, std::size_t bytes_transferred){
								if(!error){
									if(bytes_transferred > 0){
										std::string name = format_msg(*input_buf, bytes_transferred);
										std::cout << "player \"" << name << "\" connected." << std::endl;
										if (!is_name_taken(name)){
											players.emplace_back(new player(name, new client_connection(socket)));
											start_handle_client_msg(socket, players.back());
										}
										else
											; // send some error code
									}
								} else {
									std::cout << "some error occurred while trying to read name!" << std::endl << error.message() << std::endl;
								}
							});
						} else {
							std::cout << "some error occurred while trying to write greeting!" << std::endl << error.message() << std::endl;
						}
					}
				);

				//socket->receive();

			}
			start_accept();


		});
	}

	void game_server::start_handle_client_msg(std::shared_ptr<boost::asio::ip::tcp::socket> socket, std::shared_ptr<player> p){
		std::shared_ptr<boost::asio::mutable_buffer> input_buf(new boost::asio::mutable_buffer(new std::string(""), 250));
		socket->async_read_some(*input_buf, [this, socket, input_buf, p](const boost::system::error_code& error, std::size_t bytes_transferred){
			if (!error){
				if (bytes_transferred > 0){
					std::string msg = format_msg(*input_buf, bytes_transferred);
					std::cout << p->get_name() << ": " << msg << std::endl;
					process_msg(*p, msg);
				}
				start_handle_client_msg(socket, p);
			} else {
				std::cout << "some error occurred while trying to read from client!" << std::endl << error.message() << std::endl;
			}
		});
	}

	void game_server::process_msg(player& p, const std::string& msg){

		if (msg[0] == COMMAND_PREFIX) {
			std::vector<std::string> parts;
			boost::split(parts, msg, boost::is_space());
			std::string cmd = parts[0].substr(1);
			for(auto& cb_entry : callbacks){
				if(cb_entry.cmd == cmd){
					cb_entry.callback->cb(*this, p, parts);
					break;
				}
			}
		} else {
			default_callback->cb(*this, p, {msg});
		}


	}

	bool game_server::is_name_taken(const std::string& name){
		for(const auto& p : players)
			if (p->get_name() == name)
				return true;
		return false;
	}

	player& game_server::get_player(const std::string& name){
		for(auto& p : players)
			if (p->get_name() == name)
				return *p;
		throw std::exception();
	}

	challenge_mgr& game_server::get_challenge_mgr(){return chal_mgr;}

	std::vector<std::shared_ptr<player>>& game_server::get_players(){
		return players;
	}


}
