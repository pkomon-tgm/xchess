/*
 * challenge_server_callback.hpp
 *
 *  Created on: Apr 25, 2018
 *      Author: pkomon
 */

#ifndef SERVER_CHALLENGE_HPP_
#define SERVER_CHALLENGE_HPP_

#include <string>
#include <vector>
#include <map>

#include "player.hpp"

#include "server_callback.hpp"
#include "session.hpp"

namespace chess {

	class challenge_mgr;
	class game_server;

	class challenge {
		//todo: include "game configuration"

		player* challenger;
		player* challenged;
		challenge_mgr* chal_mgr;

	public:
		challenge(player& challenger, player& challenged, challenge_mgr& chal_mgr);

		const player& get_challenger() const;
		const player& get_challenged() const;

		//void accept();
	};

	class challenge_mgr {
		//const game_server* gs;

		std::map<const player*, std::vector<challenge>> challenges;
		std::map<const player*, std::vector<game_session>> sessions;

		//std::vector<challenge> challenges;
		//std::vector<game_session> sessions;

	public:
		challenge_mgr();

		void invite(player& challenger, player& challenged);
		void create_session(challenge& chal);

		bool is_challenged(const player& player);
		bool is_challenged(const std::string& name);
		std::vector<challenge> get_challenges_of(const player& player);
		//std::vector<challenge> get_challenges_by(const std::string& name);
		//std::vector<challenge> get_challenges_of(const player& player);
		//std::vector<challenge> get_challenges_of(const std::string& name);
		bool has_challenged(const player& challenger, const player& challenged);
		bool has_challenged(const std::string& challenger, const std::string& challenged);

		//std::vector<challenge>

		std::vector<game_session*> get_sessions_of(const player& player);
		std::vector<game_session*> get_sessions_with(const player& player);


		challenge& get_challenge(const player& challenger, const player& challenged);
		//session& get_sessio(const player& challenger, const player& challenged);

	};

	class challenge_server_callback : public server_callback {
	protected:
		challenge_mgr* mgr;
	public:
		challenge_server_callback(challenge_mgr& mgr);
	};

	struct challenge_invite_callback : public challenge_server_callback {
		using challenge_server_callback::challenge_server_callback;
		virtual void cb(game_server& gs, player& con, const std::vector<std::string>& args) override;
	};

	struct challenge_accept_callback : public challenge_server_callback {
		using challenge_server_callback::challenge_server_callback;
		virtual void cb(game_server& gs, player& con, const std::vector<std::string>& args) override;
	};

	class challenge_show_callback : public challenge_server_callback {
		using challenge_server_callback::challenge_server_callback;
		virtual void cb(game_server& gs, player& con, const std::vector<std::string>& args) override;
	};

	class session_show_callback : public challenge_server_callback {
		using challenge_server_callback::challenge_server_callback;
		virtual void cb(game_server& gs, player& con, const std::vector<std::string>& args) override;
	};

}

#endif /* SERVER_CHALLENGE_HPP_ */
