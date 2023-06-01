#include "common.h"

bool isValidRequirement(int r)
{
    return r == static_cast<int>(Requirement::DNS) ||
           r == static_cast<int>(Requirement::QUERY) ||
           r == static_cast<int>(Requirement::QUIT);
}

std::string readAsString(int sock)
{
    std::vector<char> inBuffer(BUFFER_SIZE);
    ssize_t bytesRead = read(sock, inBuffer.data(), inBuffer.size());

    if (bytesRead == -1) {
        throw std::system_error(errno, std::generic_category(),
                                "Failed to read from socket");
    }
    if (bytesRead == 0) {
        throw std::runtime_error("Connection closed by peer");
    }

    return std::string(inBuffer.data(), bytesRead);
}

void sendString(int sock, std::string str)
{
    size_t bytesSent = 0;
    ssize_t currentBytesSent;

    // While we haven't sent everything
    while (bytesSent < str.size()) {
        currentBytesSent =
            send(sock, str.c_str() + bytesSent, str.size() - bytesSent, 0);

        // Check if there was an error
        if (currentBytesSent == -1) {
            throw std::runtime_error("Error sending data");
        }

        bytesSent += currentBytesSent;
    }
}
