#include "headers/Client.hpp"
// #include "headers/Game.hpp"
// #include "headers/Player.hpp"
#include "headers/Server.hpp"
#include "headers/Singleton.hpp"
// #include <SFML/Graphics.hpp>

int main()
{
	Server server(5, 26950);
	server.start();
	return 0;
}