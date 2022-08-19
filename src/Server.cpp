#include "headers/Server.hpp"
// #include "headers/Client.hpp"
// #include "headers/ServerHandle.hpp"
#include "headers/Singleton.hpp"
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <thread>
#define ticks_per_second 30
#define PORT 6968

std::unordered_map<int, std::unique_ptr<User>> clients;
// std::unordered_map<int, void (*)(int, sf::Packet)> packetHandler;
unsigned int noPlayers;
enum serverPackets
{
	welcome = 1,
	spwanPlayer,
	playerPosition,
	playerRotation,
	serverFull
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
	noPlayers = 0;
	// this->listener = sf::TcpListener();
}
void Server::start()
{
	this->initServer();
	this->TCPListen();
}

void Server::initServer()
{
	for (unsigned int i = 1; i <= maxPlayers; i++)
	{
		clients.insert(std::make_pair(i, new User(i)));
	}
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
		//UNCOMMENT
		// sf::UdpSocket* usocket = new sf::UdpSocket();
		// std::cout << noPlayers << std::endl;
		if (listener.accept(*socket) == sf::Socket::Done)
		{
			std::cout << "trying to connect to :" << socket->getRemoteAddress() << std::endl;
			noPlayers++;
			//assign a client socket only if no of connections < maxPlayers client
			if (noPlayers <= maxPlayers)
			{
				// int id = clients.size() + 1;
				// clients.insert(std::make_pair(id, new User(id)));
				// // clients[id] = new User(id);
				// clients.at(id)->tcp->connect(socket);
				for (int i = 1; i <= (int)maxPlayers; i++)
				{
					if (clients[i]->tcp->socket == nullptr)
					{
						clients[i]->tcp->connect(socket);
						// std::cout << "connected to :" << socket->getRemoteAddress() << std::endl;
						break;
					}
				}
				// UNCOMMENT
				// usocket->bind(listener.getLocalPort());
				// clients.at(id)->udp->Connect(usocket, socket->getRemoteAddress());
			}
			else
			{
				std::cout << "Server is full" << std::endl;
				sf::Packet p;
				p << serverPackets::serverFull;
				p << "Server is full";
				socket->send(p);
				socket->disconnect();
				noPlayers--;
			}
		}
		else
		{
			delete socket;
		}
		//iterate all clients and update them
		for (auto it = clients.begin(); it != clients.end(); it++)
		{
			if (it->second->tcp->socket != nullptr)
			{
				it->second->tcp->update();
			}
			// it->second->tcp->update();
			//UNCOMMENT
			// it->second->udp->update();
		}
		//update gameLogic
		GameLogic::update();
		// float currentTime = clock.restart().asSeconds();
		// float fps = 1.f / (currentTime - lastTime);
		// lastTime = currentTime;
		// std::cout << "Running at " << fps << " ticks" << std::endl;
		//sleep for ticks time
		sf::sleep(sf::milliseconds(1000 / ticks_per_second));
	}
}

//User stuff
User::User(int id)
{
	this->id = id;
	this->tcp = new TCP(id);
	this->udp = new UDP(id);
}

void User::sendIntoGame(std::string username)
{
	this->player = new Player(id, username, sf::Vector2f(400, 300));
	std::cout << "clients size " << clients.size() << std::endl;
	int x = 0;
	for (unsigned int i = 1; i <= clients.size(); i++)
	{
		if (clients[i]->player != nullptr)
		{
			if (clients[i]->id != this->id)
			{
				// send player to client
				x++;
				ServerSend::spwanPlayer(this->id, *clients[i]->player);
				std::cout << "spwan player at clientno: " << this->id << " " << clients[i]->player->getUsername() << std::endl;
			}
		}
	}
	std::cout << "loop ran = " << x << std::endl;
	for (unsigned int i = 1; i <= clients.size(); i++)
	{
		if (clients[i]->player != nullptr)
		{
			for (unsigned int j = 1; j <= clients.size(); j++)
			{
				if (clients[j]->player != nullptr)
				{
					// send player to client
					ServerSend::spwanPlayer(clients[j]->id, *clients[i]->player);
					std::cout << "spwan player at clientno: " << clients[j]->id << " " << clients[i]->player->getUsername() << std::endl;
				}
			}
		}
	}
	//spwan players correctly
	// for (auto it = clients.begin(); it != clients.end(); it++)
	// {
	// 	if (it->second->player != nullptr)
	// 	{
	// 		if (it->second->id != this->id)
	// 		{
	// 			ServerSend::spwanPlayer(this->id, *it->second->player);
	// 			std::cout << "spwan player at clientno: " << this->id << " " << it->second->player->getUsername() << std::endl;
	// 		}
	// 	}
	// }

	// for (auto it = clients.begin(); it != clients.end(); it++)
	// {
	// 	if (it->second->player != nullptr)
	// 	{
	// 		ServerSend::spwanPlayer(it->second->id, *it->second->player);
	// 		std::cout << "spwan player at clientno: " << it->second->id << " " << it->second->player->getUsername() << std::endl;
	// 	}
	// }
}

void User::disconnet()
{
	std::cout << "player " << this->tcp->socket->getRemoteAddress() << "disconnected" << std::endl;
	noPlayers--;
	player = nullptr;
	this->tcp->close();
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
	this->socket = nullptr;
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
			clients[this->id]->disconnet();
			break;
	}
}

void TCP::update()
{
	this->recieve();
}

void TCP::recieve()
{
	// char buffer[1024];
	// std::size_t received;
	// if (this->socket->receive(buffer, sizeof(buffer), received) == sf::Socket::Done)
	// {
	// 	if (received > 0)
	// 	{
	// 		std::cout << "Recieved: " << buffer << " From Client " << this->id << std::endl;
	// 	}
	// }
	sf::Packet pp;
	if (this->socket != nullptr)
	{
		while (this->socket->receive(pp) == sf::Socket::Done)
		{
			if (pp.getDataSize() > 0)
			{
				this->HandlePacket(pp);
			}
			else
			{
				std::cout << "not valid packet" << std::endl;
				clients[this->id]->disconnet();
			}
		}
		if (this->socket->receive(pp) == sf::Socket::Disconnected)
		{
			clients[this->id]->disconnet();
			// noPlayers--;
		}
	}
}
//UDP stuff
UDP::UDP(int id)
{
	this->id = id;
}
void UDP::Connect(sf::UdpSocket* socket, sf::IpAddress adress)
{
	this->usocket = socket;
	this->senderIp = adress;
	this->usocket->setBlocking(false);
	this->usocket->bind(26950);
	std::cout << "Client " << this->id << " connected via udp" << std::endl;
}

void UDP::send(sf::Packet packet)
{
	this->usocket->send(packet, this->senderIp, (unsigned short)this->port);
}

void UDP::Recieve()
{
	//IpAdress any then port as 0
	sf::Packet packet;
	sf::IpAddress sender;
	unsigned short senderPort = 26950;
	// while (this->usocket->receive(packet, sender, senderPort) == sf::Socket::Done)
	// {
	// 	if (packet.getDataSize() > 0)
	// 	{
	// 		this->HandlePacket(packet);
	// 	}
	// }
	char buffer[1024];
	std::size_t received;
	if (this->usocket->receive(buffer, sizeof(buffer), received, sender, senderPort) == sf::Socket::Done)
	{
		if (received > 0)
		{
			std::cout << "Recieved: " << buffer << " From Client " << this->id << std::endl;
		}
	}
}

void UDP::HandlePacket(sf::Packet& packet)
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

void UDP::update()
{
	this->Recieve();
}

void UDP::close()
{
	this->usocket->unbind();
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
	if (this->inputs[0] == true)
	{
		this->position.y -= vel;
	}
	if (this->inputs[1] == true)
	{
		this->position.x -= vel;
	}
	if (this->inputs[2] == true)
	{
		this->position.y += vel;
	}
	if (this->inputs[3] == true)
	{
		this->position.x += vel;
	}
	ServerSend::playerPosition(*this);
	ServerSend::playerRotation(*this);
}
void Player::move(sf::Vector2f direction)
{
	//print direction;
	direction.x = direction.x / 10;
	// std::cout << direction.x << " " << direction.y << std::endl;
	// this->position += direction;
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
	std::cout << "recieved from Client " << clientIdCheck << " " << clientId << std::endl;
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
	// int size;
	// packet >> size;
	int* inputs = new int[4];
	for (int i = 0; i < 4; i++)
	{
		packet >> inputs[i];
		// std::cout << inputs[i] << " ";
	}
	bool* inputts = new bool[4];
	for (int i = 0; i < 4; i++)
	{
		inputts[i] = false;
	}
	//if 1 true else false
	if (inputs[0] == 1)
	{
		inputts[0] = true;
	}
	if (inputs[1] == 1)
	{
		inputts[1] = true;
	}
	if (inputs[2] == 1)
	{
		inputts[2] = true;
	}
	if (inputs[3] == 1)
	{
		inputts[3] = true;
	}
	float rotation;
	// std::cout << std::endl;
	//print inputs
	packet >> rotation;
	// std::cout << clientId << std::endl;
	clients[clientId]->player->setInput(inputts, rotation);
}
//server send stuff
void ServerSend::sendTcpData(int clientId, sf::Packet packet)
{
	clients[clientId]->tcp->send(packet);
}

void ServerSend::sendTcpDataAll(sf::Packet packet)
{
	for (auto& client : clients)
	{
		if (client.second->player != nullptr)
		{
			client.second->tcp->send(packet);
		}
	}
}

void ServerSend::welcome(int clientId, std::string str)
{
	sf::Packet packet;
	packet << (int)serverPackets::welcome;
	packet << clientId;
	packet << str;
	sendTcpData(clientId, packet);
}

void ServerSend::spwanPlayer(int clientId, Player& player)
{
	sf::Packet packet;
	packet << (int)serverPackets::spwanPlayer;
	// std::cout << "spawning player Id: " << clientId << std::endl;
	// std::cout << "------------------" << std::endl;
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
	packet << (int)player.position.x;
	packet << (int)player.position.y;
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
		if (client.second->player != nullptr)
		{
			client.second->player->update();
			// std::cout << "updating player: " << client.second->id << std::endl;
		}
	}
}