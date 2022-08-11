#include "headers/Server.hpp"
// #include "headers/Client.hpp"
// #include "headers/ServerHandle.hpp"
#include "headers/Singleton.hpp"
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#define ticks_per_second 60

std::unordered_map<int, std::unique_ptr<User>> clients;
// std::unordered_map<int, void (*)(int, sf::Packet)> packetHandler;

enum serverPackets
{
	welcome = 1,
	spwanPlayer,
	playerPosition,
	playerRotation,
};

enum clientPackets
{
	welcomeRecieved = 1,
	playerMovement
};

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
	// packetHandler.insert(std::make_pair((int)clientPackets::welcomeRecieved, &ServerHandle::welcomeRecieved));
	// packetHandler.insert(std::make_pair((int)clientPackets::playerMovement, &ServerHandle::playerMovement));
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
				clients.insert(std::make_pair(id, new User(id)));
				// clients[id] = new User(id);
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
		//update gameLogic
		GameLogic::update();

		//sleep for ticks time
		sf::sleep(sf::milliseconds(1000 / ticks_per_second));
	}
}

//User stuff
User::User(int id)
{
	this->id = id;
	this->tcp = new TCP(id);
}
void User::sendIntoGame(std::string username)
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
//tcp stuff

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
	ServerSend::welcome(this->id, "Welcome to Server!");
	// packet << "Welcome to the server";
	// this->send(packet);
}

void TCP::send(sf::Packet packet)
{
	this->socket->send(packet);
}

void TCP::HandlePacket(sf::Packet& packet)
{
	int packetID;
	packet >> packetID;
	if (!packetID)
	{
		std::cout << "not valid packet" << std::endl;
		return;
	}
	switch (packetID)
	{
		case (int)clientPackets::welcomeRecieved:
			ServerHandle::welcomeRecieved(this->id, packet);
			break;
		case (int)clientPackets::playerMovement:
			ServerHandle::playerMovement(this->id, packet);
			break;
		default:
			std::cout << "not valid packet from client: " << this->id << std::endl;
			break;
	}
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
//player stuff

Player::Player(int id, std::string name, sf::Vector2f spwan)
{
	this->id = id;
	this->username = name;
	this->position = spwan;
	inputs = new bool[4];
	for (int i = 0; i < 4; i++)
	{
		inputs[i] = false;
	}
}

void Player::update()
{
	sf::Vector2f inputDirectoin = sf::Vector2f(0, 0);
	if (inputs[0])
	{
		inputDirectoin.x -= vel;
	}
	if (inputs[1])
	{
		inputDirectoin.x += vel;
	}
	if (inputs[2])
	{
		inputDirectoin.y -= vel;
	}
	if (inputs[3])
	{
		inputDirectoin.y += vel;
	}
	move(inputDirectoin);
}
void Player::move(sf::Vector2f direction)
{
	this->position += direction;
	ServerSend::playerPosition(*this);
	ServerSend::playerRotation(*this);
}
void Player::setInput(bool* inputs, float rotation)
{
	this->inputs = inputs;
	this->rotation = rotation;
}

//server Handle stuff
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
//server send stuff
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

void ServerSend::spwanPlayer(int clientId, Player& player)
{
	sf::Packet packet;
	packet << (int)serverPackets::spwanPlayer;
	packet << player.id;
	packet << player.username;
	packet << player.position.x;
	packet << player.position.y;
	packet << player.rotation;
	sendTcpData(clientId, packet);
}

void ServerSend::playerPosition(Player& player)
{
	sf::Packet packet;
	packet << (int)serverPackets::playerPosition;
	packet << player.id;
	packet << player.position.x;
	packet << player.position.y;
	packet << player.rotation;
	sendTcpDataAll(packet);
}

void ServerSend::playerRotation(Player& player)
{
	sf::Packet packet;
	packet << (int)serverPackets::playerRotation;
	packet << player.id;
	packet << player.rotation;
	sendTcpDataAll(packet);
}

//gamelogic stuff
void GameLogic::update()
{
	for (auto& client : clients)
	{
		client.second->player->update();
	}
}