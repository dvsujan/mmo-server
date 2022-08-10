#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "ServerSend.hpp"
#include "Singleton.hpp"
#include <SFML/Network.hpp>
#include <string>

class Player
{
private:
	int id;
	std::string username;
	bool* inputs;
	sf::Vector2f position;
	float rotation = 0;
	float vel = 2;

public:
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
	// void move(sf::Vector2f inputDirectoin);
};
#endif