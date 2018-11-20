/*
 * challenge.cpp
 *
 *  Created on: Apr 26, 2018
 *      Author: pkomon
 */

#include "challenge.hpp"

#include <map>

#include "session.hpp"
#include "game_server.hpp"

#include "../chess_ruleset.hpp"
#include "../game.hpp"

namespace chess {

	challenge::challenge(player& challenger, player& challenged, challenge_mgr& chal_mgr):
			challenger{&challenger}, challenged{&challenged}, chal_mgr{&chal_mgr} {}

	const player& challenge::get_challenger() const {return *challenger;}
	const player& challenge::get_challenged() const {return *challenged;}


	/*void challenge::accept(){
		game_session(chess::game{new chess::chess_ruleset()}, *challenger, *challenged); //todo: dont create default everytime
	}*/

	challenge_mgr::challenge_mgr(): challenges{} {}

	void challenge_mgr::invite(player& challenger, player& challenged){
		challenges.try_emplace(&challenged, std::vector<challenge>{});
		challenges[&challenged].emplace_back(challenger, challenged, *this);
	}

	void challenge_mgr::create_session(challenge& chal){
		sessions.try_emplace(&chal.get_challenged(), std::vector<game_session>{});
		sessions[&chal.get_challenged()].emplace_back(new chess::chess_ruleset(), chal.get_challenger(), chal.get_challenged());
	}


//	bool challenge_mgr::is_challenged(const player& player){
//
//	}
//	bool challenge_mgr::is_challenged(const std::string& name){
//
//	}
//	std::vector<challenge> challenge_mgr::get_challenges_from(const player& challenger){
//
//	}
//	std::vector<challenge> challenge_mgr::get_challenges_from(const std::string& challenger){
//
//	}
	std::vector<challenge> challenge_mgr::get_challenges_of(const player& challenged){
		return challenges[&challenged];
	}
/*
	std::vector<challenge> challenge_mgr::get_challenges_(const std::string& challenged){
		std::vector<challenge> out;
		for(const auto& chal : challenges)
			if(chal.get_challenged().get_name() == challenged)
				out.push_back(chal);
		return out;
	}
*/
	bool challenge_mgr::has_challenged(const player& challenger, const player& challenged){
		for(const auto& chal : challenges[&challenged]){
			if(chal.get_challenger() == challenger)
				return true;
		}
		return false;
	}

/*	bool challenge_mgr::has_challenged(const std::string& challenger, const std::string& challenged){
		for(const auto& chal : challenges[&challenger]){
			if(chal.get_challenger().get_name() == challenger && chal.get_challenged().get_name() == challenged)
				return true;
		}
		return false;
	}*/

	std::vector<game_session*> challenge_mgr::get_sessions_of(const player& player){
		std::vector<game_session*> out;
		for (auto& sess : sessions[&player])
			out.push_back(&sess);
		return out;
	}

	std::vector<game_session*> challenge_mgr::get_sessions_with(const player& player){
		std::vector<game_session*> out;
		for (auto& entry : sessions){
			for (auto& session : entry.second){
				if (session.get_challenger() == player || session.get_challenged() == player)
					out.push_back(&session);
			}
		}
		return out;
	}

	challenge& challenge_mgr::get_challenge(const player& challenger, const player& challenged){
		for(auto& chal : get_challenges_of(challenged)){
			if(chal.get_challenger() == challenger)
				return chal;
		}
		throw std::exception();
	}

	challenge_server_callback::challenge_server_callback(challenge_mgr& mgr): mgr{&mgr} {}


	void challenge_invite_callback::cb(game_server& gs, player& con, const std::vector<std::string>& args){
		if(args.size() < 2){
			throw std::exception(); //todo implement exception for wrong parameter to command
		}

		if(!gs.is_name_taken(args[1]))
			throw std::exception(); //todo implement exception for player not found

		if (!mgr->has_challenged(con, gs.get_player(args[1]))){
			mgr->invite(con, gs.get_player(args[1]));
		} else {
			//nope, challenge to this player already pending ... only one challenge to same player at a time?
		}

	}

	void challenge_accept_callback::cb(game_server& gs, player& con, const std::vector<std::string>& args){
		if(args.size() < 2){
			throw std::runtime_error("too few args"); //todo implement own exception for wrong parameter to command
		}

		if(!gs.is_name_taken(args[1]))
			throw std::runtime_error("player '" + args[1] + "' not found"); //todo implement exception for player not found

		if (mgr->has_challenged(gs.get_player(args[1]), con)){
			//game_session* session = mgr->get_challenge(gs.get_player(args[1]), con).accept();
			mgr->create_session(mgr->get_challenge(gs.get_player(args[1]), con));
		} else {
			//nope, challenge to this player already pending ... only one challenge to same player at a time?
		}

	}

	void challenge_show_callback::cb(game_server& gs, player& con, const std::vector<std::string>& args){
		std::string answer{"Your were challenged by:\n"};
		for(const auto& chal : mgr->get_challenges_of(con)){
			answer += "\t- " + chal.get_challenger().get_name() + " \n";
		}
		con.get_client_connection().get_socket().send(boost::asio::buffer(answer));
	}

	void session_show_callback::cb(game_server& gs, player& con, const std::vector<std::string>& args){
		std::string answer{"Your have sessions with:\n"};
		auto session_list = mgr->get_sessions_with(con);
		for(std::size_t i = 0; i < session_list.size(); ++i){
			const auto& sess = session_list[i];
			const player& other = sess->get_challenger() == con ? sess->get_challenged() : sess->get_challenger();
			answer += std::string("\t(") + std::to_string(i) + std::string(") ") + other.get_name() + std::string(" \n");
		}
		con.get_client_connection().get_socket().send(boost::asio::buffer(answer));
	}


}


