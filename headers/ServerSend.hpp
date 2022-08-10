#ifndef SERVERSEND_HPP
#define SERVERSEND_HPP
#include "Player.hpp"
#include <SFML/Network.hpp>
#include <iostream>
#include <string>

class ServerSend
{
public:
	static void sendTcpData(int clientId, sf::Packet packet);
	static void sendTcpDataAll(sf::Packet packet);
	static void welcome(int clientId, std::string str);
	static void spwanPlayer(int clientId, Player player);
	static void playerPosition(Player player);
	static void playerRotation(Player player);
};

#endif