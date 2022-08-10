#include "../headers/Client.hpp"
#include "../headers/ServerSend.hpp"
#include "../headers/Singleton.hpp"
#include <SFML/Network.hpp>
#include <unordered_map>

Client::Client(int id)
{
	this->id = id;
	this->tcp = new TCP(id);
	//create thread for this class
}

TCP::TCP(int id)
{
	this->id = id;
	this->recievePacket = sf::Packet();
}

void TCP::close()
{
	this->socket->disconnect();
}

void TCP::connect(sf::TcpSocket* socket)
{
	this->socket = socket;
	this->socket->setBlocking(false);
	std::cout << "Client " << this->id << " connected" << std::endl;
	sf::Packet packet;
	packet << "Welcome to the server";
	this->send(packet);
	//thread for this class
	// sf::Thread thread(&TCP::recieve, this);
	// thread.launch();
}

void TCP::send(sf::Packet packet)
{
	this->socket->send(packet);
}

// void TCP::recieve()
// {
// 	// sf::Packet p;
// 	char buffer[1024];
// 	std::size_t received;
// 	// if (this->socket->receive(p) == sf::Socket::Done)
// 	// {
// 	// 	if (p.getDataSize() > 0)
// 	// 	{
// 	// 		HandlePacket(p);
// 	// 	}
// 	// }
// 	while (true)
// 	{
// 		if (this->socket->receive(buffer, sizeof(buffer), received) == sf::Socket::Done)
// 		{
// 			if (received > 0)
// 			{
// 				this->recievePacket.append(buffer, received);
// 				std::cout << "Recieved: " << buffer << std::endl;
// 			}
// 		}
// 	}
// }

void TCP::HandlePacket(sf::Packet& packet)
{
	int packetID;
	packet >> packetID;
	if (!packetID)
	{
		std::cout << "not valid packet" << std::endl;
		return;
	}
	packetHandler[packetID](this->id, packet);
}

void TCP::update()
{
	this->recieve();
}

void TCP::recieve()
{
	char buffer[1024];
	std::size_t received;
	if (this->socket->receive(buffer, sizeof(buffer), received) == sf::Socket::Done)
	{
		if (received > 0)
		{
			std::cout << "Recieved: " << buffer << " From Client " << this->id << std::endl;
		}
	}
	else if (this->socket->receive(this->recievePacket) == sf::Socket::Done)
	{
		if (this->recievePacket.getDataSize() > 0)
		{
			this->HandlePacket(this->recievePacket);
		}
	}
}

void Client::sendIntoGame(std::string username)
{
	this->player = new Player(id, username, sf::Vector2f(0, 0));
	for (auto& client : clients)
	{

		if (client.second->player != nullptr)
		{
			if (client.first != this->id)
			{
				//send player to client
				ServerSend::spwanPlayer(id, *this->player);
			}
		}
	}
	for (auto& client : clients)
	{
		if (client.second->player != nullptr)
		{
			ServerSend::spwanPlayer(client.first, *this->player);
		}
	}
}