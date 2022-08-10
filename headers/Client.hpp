#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "SFML/Network.hpp"
#include "headers/Player.hpp"
#include <SFML/System/Thread.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

class TCP
{
public:
	int id;
	sf::TcpSocket* socket;
	sf::Packet recievePacket;
	TCP(int id);
	void connect(sf::TcpSocket* socket);
	void send(sf::Packet packet);
	void recieve();
	void HandlePacket(sf::Packet& packet);
	void close();
	//create a function that checks if packet is recieved whithout blocking constantly and if it is then handle the packet
	void update();
};

class Client
{
public:
	int id;
	TCP* tcp;
	Client(int id);
	Player* player;
	void sendIntoGame(std::string username);
};

#endif