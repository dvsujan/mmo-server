#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "headers/Server.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>

class Physics
{
public:
	static bool lineLine(sf::Vector2i p1, sf::Vector2i p2, sf::Vector2i p3, sf::Vector2i p4);
	static bool lineRect(sf::Vector2i p1, sf::Vector2i p2, PRect rect);
};

#endif