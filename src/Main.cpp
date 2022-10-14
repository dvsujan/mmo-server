#include "headers/Client.hpp"
// #include "headers/Game.hpp"
// #include "headers/Player.hpp"
#include "headers/Server.hpp"
#include "headers/Singleton.hpp"
// #include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>

using namespace std;

int main()
{
	Server server(10, 26950);
	server.start();
	return 0;
}