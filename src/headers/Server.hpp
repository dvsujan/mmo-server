#ifndef SERVER_HPP
#define SERVER_HPP
#include "./Singleton.hpp"
#include <SFML/Network.hpp>
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

class UDP
{
public:
	int id;
	sf::UdpSocket* usocket;
	sf::IpAddress senderIp;
	UDP(int id);
	const unsigned short port = 6969;
	void Connect(sf::UdpSocket* socket, sf::IpAddress adress);
	void send(sf::Packet packet);
	void Recieve();
	void HandlePacket(sf::Packet& packet);
	void close();
	void update();
};

class User
{
public:
	int id;
	TCP* tcp;
	UDP* udp;
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
};

class ServerHandle
{
public:
	static void welcomeRecieved(int clientId, sf::Packet& packet);
	static void playerMovement(int clientId, sf::Packet& packet);
};

class Player
{

public:
	Player()
	{}
	int id;
	std::string username;
	bool* inputs;
	sf::Vector2f position;
	float rotation = 0;
	float vel = 5;
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