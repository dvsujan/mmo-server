#ifndef SERVER_HPP
#define SERVER_HPP
#include "./Singleton.hpp"
#include <SFML/Network.hpp>
#include <iostream>
#include <unordered_map>

class Player;
class ServerHandle;
class ServerSend;
class User;
class TCP;
class GameLogic;
class UDP;

class TCP
{
public:
	int id;
	sf::TcpSocket* socket = nullptr;
	sf::Packet recievePacket;
	TCP(int id);
	void connect(sf::TcpSocket* socket);
	void send(sf::Packet packet);
	void recieve();
	void HandlePacket(sf::Packet& packet);
	void close();
	void update();
};

// class UDP
// {
// public:
// 	int id;
// 	sf::UdpSocket* usocket;
// 	sf::IpAddress senderIp;
// 	UDP(int id);
// 	const unsigned short port = 6969;
// 	void Connect(sf::UdpSocket* socket, sf::IpAddress adress);
// 	void send(sf::Packet packet);
// 	void Recieve();
// 	void HandlePacket(sf::Packet& packet);
// 	void close();
// 	void update();
// };

class User
{
public:
	int id;
	TCP* tcp;
	// UDP* udp;
	User(int id);
	Player* player = NULL;
	void sendIntoGame(std::string username);
	void disconnet();
};

class ServerSend
{
public:
	static void sendTcpData(int clientId, sf::Packet packet);
	static void sendTcpDataAll(sf::Packet packet);
	static void welcome(int clientId, std::string str);
	static void spwanPlayer(int clientId, Player& player);
	static void playerPosition(Player& player);
	static void playerRotation(Player& player);
	static void takeDamage(Player& player);
	static void killPlayer(int clientId);
	static void playerScore(Player& player);
	static void playerDisconnected(int clientId);
};

class ServerHandle
{
public:
	static void welcomeRecieved(int clientId, sf::Packet& packet);
	static void playerMovement(int clientId, sf::Packet& packet);
	static void handleShoot(int clientId, sf::Packet& packet);
};

class PRect
{
public:
	sf::Vector2f p1;
	sf::Vector2f p2;
	sf::Vector2f p3;
	sf::Vector2f p4;
	PRect();
	PRect(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4);
	void print()
	{
		std::cout << "p1: " << p1.x << ", " << p1.y << std::endl;
		std::cout << "p2: " << p2.x << ", " << p2.y << std::endl;
		std::cout << "p3: " << p3.x << ", " << p3.y << std::endl;
		std::cout << "p4: " << p4.x << ", " << p4.y << std::endl;
	}
};

class Player
{
public:
	Player()
	{}
	int id;
	int score;
	std::string username;
	bool isAlive = true;
	bool* inputs;
	sf::Vector2f position;
	sf::Vector2f size;
	float rotation = 0;
	float vel = 5;
	int health = 100;
	Player(int id, std::string name, sf::Vector2f spwan);
	void update();
	void setInput(bool* inpts, float rotation);
	int getId()
	{
		return id;
	}
	std::string getUsername()
	{
		return username;
	}
	sf::Vector2f getPosition()
	{
		return position;
	}
	float getRotation()
	{
		return rotation;
	}
	void move(sf::Vector2f inputDirectoin);
	void addScore();
	PRect getRect();
	void TakeDamage(int damage);
};

class Server
{
private:
	unsigned int maxPlayers;
	int port;
	sf::TcpListener listener;
	// int noPlayers;

public:
	Server(int maxPlayers, int port);
	void TCPListen();
	void initServer();
	void start();
	void update();
};

class GameLogic
{
public:
	static void update();
};

#endif