#ifndef SERVERHANDLE_HPP
#define SERVERHANDLE_HPP
#include "Server.hpp"
#include "Singleton.hpp"
#include <SFML/Network.hpp>
#include <iostream>

class ServerHandle
{
public:
	static void welcomeRecieved(int clientId, sf::Packet& packet);
	static void playerMovement(int clientId, sf::Packet& packet);
};

#endif