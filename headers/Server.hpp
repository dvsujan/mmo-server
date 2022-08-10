#ifndef SERVER_HPP
#define SERVER_HPP
#include "Client.hpp"
#include <SFML/Network.hpp>
#include <unordered_map>

class Server
{
private:
	unsigned int maxPlayers;
	int port;
	sf::TcpListener listener;

public:
	Server(int maxPlayers, int port);
	void TCPListen();
	void initServer();
	void start();
	void update();
};

#endif