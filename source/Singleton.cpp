#include "../headers/Singleton.hpp"

std::unordered_map<int, std::unique_ptr<Client>> clients;

std::unordered_map<int, void (*)(int, sf::Packet)> packetHandler;