#include "../headers/Server.hpp"
#include "../headers/Client.hpp"
#include "../headers/ServerHandle.hpp"
#include "../headers/Singleton.hpp"
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#define ticks_per_second 60

Server::Server(int maxPlayers, int port)
{

	std::cout << "Server Started" << std::endl;
	std::cout << "Running at " << ticks_per_second << " ticks per second" << std::endl;
	this->maxPlayers = maxPlayers;
	this->port = port;
	// this->listener = sf::TcpListener();
}
void Server::start()
{
	this->initServer();
	this->TCPListen();
}

void Server::initServer()
{
	// for (unsigned int i = 1; i <= maxPlayers; i++)
	// {
	// 	clients.insert(std::make_pair(i, new Client(i)));
	// }
	packetHandler.insert(std::make_pair((int)clientPackets::welcomeRecieved, &ServerHandle::welcomeRecieved));
	packetHandler.insert(std::make_pair((int)clientPackets::playerMovement, &ServerHandle::playerMovement));
	std::cout << "Packets initalized" << std::endl;
}

void Server::TCPListen()
{
	//connect to multiple clients at same time and no of members in server should be less than maxPlayers
	listener.listen(port);
	listener.setBlocking(false);

	while (true)
	{
		sf::TcpSocket* socket = new sf::TcpSocket();
		if (listener.accept(*socket) == sf::Socket::Done)
		{
			std::cout << "trying to connect to :" << socket->getRemoteAddress() << std::endl;
			//assign a client socket only if no of connections < maxPlayers client
			if (clients.size() < maxPlayers)
			{
				//run each client in it's own threads

				int id = clients.size() + 1;
				clients.insert(std::make_pair(id, new Client(id)));
				clients.at(id)->tcp->connect(socket);
			}
			else
			{
				std::cout << "Server is full" << std::endl;
				socket->disconnect();
			}
		}
		else
		{
			delete socket;
		}
		//iterate all clients and update them
		for (auto it = clients.begin(); it != clients.end(); it++)
		{
			it->second->tcp->update();
		}
		sf::sleep(sf::milliseconds(1000 / ticks_per_second));
	}
}
