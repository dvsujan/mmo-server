#ifndef SINGLETON_HPP
#define SINGLETON_HPP
#include "Client.hpp"
#include <SFML/Network.hpp>
#include <iostream>
#include <memory>
#include <unordered_map>
extern std::unordered_map<int, std::unique_ptr<Client>> clients;
extern std::unordered_map<int, void (*)(int, sf::Packet)> packetHandler;

enum serverPackets
{
	welcome = 1,
	spwanPlayer,
	playerPosition,
	playerRotation,
};

enum clientPackets
{
	welcomeRecieved = 1,
	playerMovement
};

#endif