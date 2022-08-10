#include "../headers/Player.hpp"

Player::Player(int id, std::string name, sf::Vector2f spwan)
{
	this->id = id;
	this->username = name;
	this->position = spwan;
	inputs = new bool[4];
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
// void Player::move(sf::Vector2f inputDirectoin)
// {
// 	position += inputDirectoin;
// 	// ServerSend::playerPosition(*this);
// 	// ServerSend::playerRotation(*this);
// }
void Player::setInput(bool* inpts, float rotation)
{
	this->inputs = inpts;
	this->rotation = rotation;
}
