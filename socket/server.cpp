#include <netdb.h>
#include <netinet/in.h>

#include <exception>
#include <fstream>
#include <stdexcept>
#include <unordered_map>

#include "common.h"

using emailMap = std::unordered_map<std::string, std::string>;

emailMap loadEmailDirectory()
{
    emailMap emailDirectory;
    std::ifstream file("query.txt");
    std::string sid, email;

    while (file >> sid >> email) {
        emailDirectory[sid] = email;
    }

    file.close();

    return emailDirectory;
}

int main()
{
    int server_fd;
    sockaddr_in address{};
    socklen_t addrlen = sizeof(address);
    emailMap emailDirectory;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "socket failed\n";
        return -1;
    }

    address.sin_family = PF_INET;
    address.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &address.sin_addr) <= 0) {
        std::cerr << "Invalid address\n";
        return -1;
    }

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "bind failed\n";
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        std::cerr << "listen failed\n";
        return -1;
    }

    emailDirectory = loadEmailDirectory();

    while (true) {
        int sock;

        std::cout << "Waiting for a connection...\n";
        sock = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (sock < 0) {
            std::cerr << "accept failed\n";
            return -1;
        }

        std::cout << "Connected\n";

        while (true) {
            Requirement req;
            ssize_t bytesRead;
            std::vector<char> inBuffer(BUFFER_SIZE);
            std::string outBuffer;

            std::cout << "Waiting for a requirement...\n";
            outBuffer = "What's your requirement? 1. DNS 2. QUERY 3. QUIT : ";
            send(sock, outBuffer.c_str(), outBuffer.size(), 0);
            bytesRead = read(sock, inBuffer.data(), inBuffer.size());

            req = static_cast<Requirement>(
                std::stoi(std::string(inBuffer.data(), bytesRead)));

            if (req == Requirement::DNS) {
                int status;
                std::string url;
                struct addrinfo hints {
                }, *res;
                hints.ai_family = AF_INET;  // AF_INET means IPv4
                hints.ai_socktype = SOCK_STREAM;

                std::cout << "Handling DNS...\n";
                outBuffer = "Input URL address : ";
                std::cout << outBuffer;
                send(sock, outBuffer.c_str(), outBuffer.size(), 0);

                bytesRead = read(sock, inBuffer.data(), inBuffer.size());
                url = std::string(inBuffer.data(), bytesRead);
                std::cout << url << '\n';

                status = getaddrinfo(url.c_str(), NULL, &hints, &res);
                if (status == 0) {
                    char ipstr[INET6_ADDRSTRLEN];
                    struct sockaddr_in *ipv4 =
                        (struct sockaddr_in *)res->ai_addr;
                    inet_ntop(res->ai_family, &(ipv4->sin_addr), ipstr,
                              sizeof(ipstr));
                    outBuffer = std::string(ipstr);
                } else {
                    std::cerr << "getaddrinfo: " << gai_strerror(status)
                              << '\n';
                    outBuffer = "Error resolving URL";
                }

                std::cout << "URL address : " << outBuffer << '\n';

                freeaddrinfo(res);
            } else if (req == Requirement::QUERY) {
                std::string sid, email;
                emailMap::iterator it;

                std::cout << "Handling QUERY...\n";
                outBuffer = "Input student ID : ";
                std::cout << outBuffer;
                send(sock, outBuffer.c_str(), outBuffer.size(), 0);

                bytesRead = read(sock, inBuffer.data(), inBuffer.size());
                sid = std::string(inBuffer.data(), bytesRead);
                std::cout << sid << '\n';

                it = emailDirectory.find(sid);

                if (it != emailDirectory.end()) {
                    email = it->second;
                    outBuffer = email;
                } else {
                    outBuffer = "No such student ID";
                }

                std::cout << "Student's email : " << outBuffer << '\n';
            } else {
                std::cout << "Quitting...\n";
                break;
            }

            send(sock, outBuffer.c_str(), outBuffer.size(), 0);
        }

        close(sock);
    }

    return 0;
}
