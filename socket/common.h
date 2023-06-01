#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <system_error>
#include <vector>

constexpr int PORT = 1234;
constexpr int BUFFER_SIZE = 1024;
constexpr char SERVER_IP[] = "127.0.0.1";

// An enum class defining the different types of client requirements
enum class Requirement { DNS = 1, QUERY = 2, QUIT = 3 };

// Returns true if the given int corresponds to a valid Requirement
bool isValidRequirement(int r);

// Read from the socket and convert the result into std::string
std::string readAsString(int);

// Send a std::string to the socket
void sendString(int, std::string);
