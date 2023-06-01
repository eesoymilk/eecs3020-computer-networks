#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

constexpr int PORT = 1234;
constexpr int BUFFER_SIZE = 1024;
constexpr char SERVER_IP[] = "127.0.0.1";

enum class Requirement { DNS = 1, QUERY = 2, QUIT = 3 };

inline bool isValidRequirement(int r)
{
    return r == static_cast<int>(Requirement::DNS) ||
           r == static_cast<int>(Requirement::QUERY) ||
           r == static_cast<int>(Requirement::QUIT);
}
