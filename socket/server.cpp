#include <netdb.h>
#include <netinet/in.h>

#include <fstream>
#include <unordered_map>

#include "common.h"

using emailMap = std::unordered_map<std::string, std::string>;

int createServerSocket();
void handleClientSession(int, const emailMap &);
void getIpAddress(std::string, std::string &);
void getStudentEmail(const emailMap &, std::string, std::string &);
emailMap loadEmailDirectory();

int main()
{
    try {
        int serverSocket = createServerSocket();
        emailMap emailDirectory = loadEmailDirectory();
        while (true) {
            int clientSocket;

            std::cout << "Waiting for a connection...\n";

            clientSocket = accept(serverSocket, nullptr, nullptr);
            if (clientSocket < 0) {
                throw std::runtime_error("Accept failed");
            }
            std::cout << "Connected\n";

            handleClientSession(clientSocket, emailDirectory);

            close(clientSocket);
        }
        return 0;
    } catch (const std::runtime_error &e) {
        std::cerr << "An error occurred: " << e.what() << '\n';
        return -1;
    }
}

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

int createServerSocket()
{
    int sock;
    sockaddr_in address{};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        throw std::runtime_error("Socket creation failed");
    }

    address.sin_family = PF_INET;
    address.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &address.sin_addr) <= 0) {
        throw std::runtime_error("Invalid address");
    }

    if (bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
        throw std::runtime_error("Bind failed");
    }

    if (listen(sock, 5) < 0) {
        throw std::runtime_error("Listen failed");
    }

    return sock;
}

void handleClientSession(int sock, const emailMap &emailDirectory)
{
    Requirement req;
    ssize_t bytesRead;
    std::vector<char> inBuffer(BUFFER_SIZE);
    std::string outBuffer;

    while (true) {
        char ack[4];
        read(sock, ack, sizeof(ack));

        std::cout << "Waiting for a requirement...\n";
        outBuffer = "What's your requirement? 1. DNS 2. QUERY 3. QUIT : ";
        send(sock, outBuffer.c_str(), outBuffer.size(), 0);
        bytesRead = read(sock, inBuffer.data(), inBuffer.size());

        req = static_cast<Requirement>(
            std::stoi(std::string(inBuffer.data(), bytesRead)));

        if (req == Requirement::DNS) {
            std::string url;

            std::cout << "Handling DNS...\n";

            outBuffer = "Input URL address : ";
            std::cout << outBuffer;
            send(sock, outBuffer.c_str(), outBuffer.size(), 0);

            bytesRead = read(sock, inBuffer.data(), inBuffer.size());

            url = std::string(inBuffer.data(), bytesRead);
            std::cout << url << '\n';

            getIpAddress(url, outBuffer);
            std::cout << "IP address : " << outBuffer << '\n';
        } else if (req == Requirement::QUERY) {
            std::string sid;

            std::cout << "Handling QUERY...\n";

            outBuffer = "Input student ID : ";
            std::cout << outBuffer;
            send(sock, outBuffer.c_str(), outBuffer.size(), 0);

            bytesRead = read(sock, inBuffer.data(), inBuffer.size());
            sid = std::string(inBuffer.data(), bytesRead);
            std::cout << sid << '\n';

            getStudentEmail(emailDirectory, sid, outBuffer);

            std::cout << "Student's email : " << outBuffer << '\n';
        } else {
            std::cout << "Quitting...\n";
            break;
        }

        send(sock, outBuffer.c_str(), outBuffer.size(), 0);
    }
}

void getIpAddress(std::string url, std::string &buffer)
{
    int status;
    struct addrinfo hints {
    }, *res;

    hints.ai_family = AF_INET;  // AF_INET means IPv4
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(url.c_str(), NULL, &hints, &res);
    if (status == 0) {
        char ipstr[INET6_ADDRSTRLEN];
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
        inet_ntop(res->ai_family, &(ipv4->sin_addr), ipstr, sizeof(ipstr));
        buffer = std::string(ipstr);
        freeaddrinfo(res);
    } else {
        std::cerr << "getaddrinfo : " << gai_strerror(status) << '\n';
        buffer = "Error resolving URL";
    }
}

void getStudentEmail(const emailMap &emailDirectory, std::string sid,
                     std::string &buffer)
{
    emailMap::const_iterator it;

    if ((it = emailDirectory.find(sid)) != emailDirectory.end()) {
        buffer = it->second;
    } else {
        buffer = "No such student ID";
    }
}
