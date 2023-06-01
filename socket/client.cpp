#include "common.h"

int main()
{
    int sock;
    sockaddr_in server_addr{};
    Requirement req;
    ssize_t bytesRead;
    std::vector<char> inBuffer(BUFFER_SIZE);
    std::string outBuffer;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error\n";
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address\n";
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
        0) {
        std::cerr << "Connection failed\n";
        return -1;
    }

    while (true) {
        bytesRead = read(sock, inBuffer.data(), inBuffer.size());
        std::cout << std::string(inBuffer.data(), bytesRead);

        while (true) {
            std::getline(std::cin, outBuffer);

            try {
                int reqPrompt = std::stoi(outBuffer);

                if (!(reqPrompt == static_cast<int>(Requirement::DNS) ||
                      reqPrompt == static_cast<int>(Requirement::QUERY) ||
                      reqPrompt == static_cast<int>(Requirement::QUIT))) {
                    throw std::invalid_argument(std::to_string(reqPrompt));
                }

                req = static_cast<Requirement>(reqPrompt);
                break;

            } catch (const std::invalid_argument &ia) {
                std::cerr << "Invalid argument : " << ia.what() << '\n';
            } catch (const std::out_of_range &oor) {
                std::cerr << "Out of range : " << oor.what() << '\n';
            }

            std::cout << "Re-enter your requirement : ";
        }

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

    close(sock);

    return 0;
}
