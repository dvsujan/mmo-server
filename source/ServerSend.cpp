#include "../headers/ServerSend.hpp"
#include "../headers/Player.hpp"
#include "../headers/Server.hpp"
#include "../headers/Singleton.hpp"
#include <string>
void ServerSend::sendTcpData(int clientId, sf::Packet packet)
{
	clients[clientId]->tcp->send(packet);
}

void ServerSend::sendTcpDataAll(sf::Packet packet)
{
	for (unsigned int i = 0; i < clients.size(); i++)
	{
		clients[i]->tcp->send(packet);
	}
}

void ServerSend::welcome(int clientId, std::string str)
{
	sf::Packet packet;
	packet << str;
	clients[clientId]->tcp->send(packet);
}

void ServerSend::spwanPlayer(int clientId, Player player)
{
	sf::Packet packet;
	packet << player.getId() << player.getUsername() << player.getPosition().x << player.getPosition().y << player.getRotation();
	std::cout << clientId << std::endl;
	sendTcpDataAll(packet);
}

void ServerSend::playerPosition(Player player)
{
	sf::Packet packet;
	packet << serverPackets::playerPosition << player.getId() << player.getPosition().x << player.getPosition().y;
	sendTcpDataAll(packet);
}
void ServerSend::playerRotation(Player player)
{
	sf::Packet packet;
	packet << serverPackets::playerRotation << player.getId() << player.getRotation();
	sendTcpDataAll(packet);
}
