#include "common.h"

// Creates a client socket and attempts to connect to the server
int createClientSocket();

// Prompts the user to input a Requirement and validates the input
Requirement getClientRequirement(std::string&);

// Handles the main communication session with the server
void handleSession(int);

int main()
{
    try {
        int clientSocket = createClientSocket();

        handleSession(clientSocket);
        close(clientSocket);

        return 0;
    } catch (const std::runtime_error& e) {
        // In case of any exceptions, print the error message and return -1
        std::cerr << "An error occurred: " << e.what() << '\n';
        return -1;
    }
}

int createClientSocket()
{
    int sock;
    sockaddr_in server_addr{};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw std::runtime_error("Socket creation error");
    }

    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        throw std::runtime_error("Invalid address");
    }

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) <
        0) {
        throw std::runtime_error("Connection failed");
    }

    return sock;
}

void handleSession(int sock)
{
    Requirement req;
    std::string buffer;

    while (true) {
        sendString(sock, "ack");

        std::cout << readAsString(sock);

        req = getClientRequirement(buffer);

        sendString(sock, buffer);

        if (req == Requirement::QUIT) {
            break;
        }

        std::cout << readAsString(sock);
        std::getline(std::cin, buffer);
        sendString(sock, buffer);

        if (req == Requirement::DNS) {
            std::cout << "address get from domain name : ";
        } else if (req == Requirement::QUERY) {
            std::cout << "Email get from server : ";
        }

        std::cout << readAsString(sock) << "\n\n";
    }
}

Requirement getClientRequirement(std::string& buffer)
{
    while (true) {
        std::getline(std::cin, buffer);

        try {
            int reqPrompt = std::stoi(buffer);

            if (!isValidRequirement(reqPrompt)) {
                throw std::invalid_argument(buffer);
            }

            return static_cast<Requirement>(reqPrompt);

        } catch (const std::invalid_argument& ia) {
            std::cerr << "Invalid argument : " << ia.what() << '\n';
        } catch (const std::out_of_range& oor) {
            std::cerr << "Out of range : " << oor.what() << '\n';
        }

        std::cout << "Re-enter your requirement : ";
    }
}
