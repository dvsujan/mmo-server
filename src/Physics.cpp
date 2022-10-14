#include "headers/Physics.hpp"

bool Physics::lineLine(sf::Vector2i p1, sf::Vector2i p2, sf::Vector2i p3, sf::Vector2i p4)
{
	//check if two line are colliding with each other first line points are p1 and p2 and second line points are p3, p4
	float x1 = (float)p1.x;
	float y1 = (float)p1.y;
	float x2 = (float)p2.x;
	float y2 = (float)p2.y;
	float x3 = (float)p3.x;
	float y3 = (float)p3.y;
	float x4 = (float)p4.x;
	float y4 = (float)p4.y;
	float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
	{
		return true;
	}
	return false;
}

bool Physics::lineRect(sf::Vector2i p1, sf::Vector2i p2, PRect rect)
{
	//check if line is colliding with rectangle
	sf::Vector2i p3 = sf::Vector2i(rect.p1.x, rect.p1.y);
	sf::Vector2i p4 = sf::Vector2i(rect.p2.x, rect.p2.y);
	sf::Vector2i p5 = sf::Vector2i(rect.p3.x, rect.p3.y);
	sf::Vector2i p6 = sf::Vector2i(rect.p4.x, rect.p4.y);
	if (lineLine(p1, p2, p3, p4) || lineLine(p1, p2, p4, p5) || lineLine(p1, p2, p5, p6) || lineLine(p1, p2, p6, p3))
	{
		return true;
	}
	return false;
}
