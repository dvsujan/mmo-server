#include "../headers/GameLogic.hpp"
#include "../headers/Server.hpp"
#include <SFML/Network.hpp>
#include <SFML/System/Thread.hpp>
#include <iostream>
#include <mutex>
#define ticks 60
#define maxPlayers 4
#define port 6969

int main()
{
	Server server(4, 26950);
	// bool isRunning = true;
	// GameLogic logic;
	// sf::Thread thread(&GameLogic::update, &logic);
	//run thread 60 ticks per second thread
	// sf::Clock clock;
	// while (isRunning)
	// {
	// 	if (clock.getElapsedTime().asSeconds() > 1.0f / ticks)
	// 	{
	// 		clock.restart();
	// 		thread.launch();
	// 	}

	// 	sf::sleep(sf::milliseconds(1000 / ticks));
	// }
	server.start();
	// server.update();
	return 0;
}