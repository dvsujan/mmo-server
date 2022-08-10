#include "../headers/ServerHandle.hpp"

void ServerHandle::welcomeRecieved(int clientId, sf::Packet& packet)
{
	int clientIdCheck;
	packet >> clientIdCheck;
	if (clientIdCheck != clientId)
	{
		std::cout << "client Id mismatch" << std::endl;
	}
	std::string username;
	packet >> username;
	std::cout << "username: " << username << std::endl;
	// clientId++;
	clients[clientId]->sendIntoGame(username);
	// clients[clientId]->tcp->send(packet);
}

void ServerHandle::playerMovement(int clientId, sf::Packet& packet)
{
	int size;
	packet >> size;
	bool* inputs = new bool[size];
	for (int i = 0; i < size; i++)
	{
		bool a;
		packet >> a;
		inputs[i] = a;
	}
	float rotation;
	packet >> rotation;
	std::cout << clientId << std::endl;
	clients[clientId]->player->setInput(inputs, rotation);
}
