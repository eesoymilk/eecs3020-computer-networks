#include "common.h"

int createClientSocket();
Requirement getClientRequirement(std::string&);
void handleSession(int);

int main()
{
    int clientSocket;

    try {
        clientSocket = createClientSocket();
    } catch (const std::runtime_error& e) {
        std::cerr << "An error occurred: " << e.what() << '\n';
        return -1;
    }

    handleSession(clientSocket);
    close(clientSocket);

    return 0;
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
    ssize_t bytesRead;
    std::vector<char> inBuffer(BUFFER_SIZE);
    std::string outBuffer;

    while (true) {
        bytesRead = read(sock, inBuffer.data(), inBuffer.size());
        std::cout << std::string(inBuffer.data(), bytesRead);

        req = getClientRequirement(outBuffer);

        send(sock, outBuffer.c_str(), outBuffer.size(), 0);

        if (req == Requirement::QUIT) {
            break;
        }

        bytesRead = read(sock, inBuffer.data(), inBuffer.size());
        std::cout << std::string(inBuffer.data(), bytesRead);
        std::getline(std::cin, outBuffer);
        send(sock, outBuffer.c_str(), outBuffer.size(), 0);
        bytesRead = read(sock, inBuffer.data(), inBuffer.size());

        if (req == Requirement::DNS) {
            std::cout << "address get from domain name : ";
        } else if (req == Requirement::QUERY) {
            std::cout << "Email get from server : ";
        }

        std::cout << std::string(inBuffer.data(), bytesRead) << "\n\n";
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
