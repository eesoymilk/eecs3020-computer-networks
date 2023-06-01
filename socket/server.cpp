#include <netdb.h>
#include <netinet/in.h>

#include <fstream>
#include <unordered_map>

#include "common.h"

using EmailMap = std::unordered_map<std::string, std::string>;

// Creates a server socket and starts listening for connections
int createServerSocket();

// Handles a client session, serving DNS and QUERY requests
void handleClientSession(int, const EmailMap &);

// Resolves a given URL to an IP address
void resolveIpAddress(std::string, std::string &);

// Retrieves a student's email from the email directory based on their ID
void getStudentEmail(const EmailMap &, std::string, std::string &);

// Loads a file with student IDs and their corresponding emails
EmailMap loadEmailDirectory();

int main()
{
    try {
        int serverSocket = createServerSocket();
        EmailMap emailDirectory = loadEmailDirectory();

        while (true) {
            int clientSocket;

            std::cout << "Waiting for a connection...\n";

            clientSocket = accept(serverSocket, nullptr, nullptr);
            if (clientSocket < 0) {
                throw std::runtime_error("Accept failed");
            }
            std::cout << "Connected\n";

            try {
                handleClientSession(clientSocket, emailDirectory);
            } catch (const std::runtime_error &e) {
                std::cerr << "An error occurred: " << e.what() << '\n';
            }

            close(clientSocket);
        }
        return 0;
    } catch (const std::runtime_error &e) {
        std::cerr << "An error occurred: " << e.what() << '\n';
        return -1;
    }
}

EmailMap loadEmailDirectory()
{
    EmailMap emailDirectory;
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

void handleClientSession(int sock, const EmailMap &emailDirectory)
{
    Requirement req;
    std::string buffer;

    while (true) {
        readAsString(sock);

        std::cout << "Waiting for a requirement...\n";
        sendString(sock, "What's your requirement? 1. DNS 2. QUERY 3. QUIT : ");

        req = static_cast<Requirement>(std::stoi(readAsString(sock)));

        if (req == Requirement::DNS) {
            std::string url;

            std::cout << "Handling DNS...\n";

            buffer = "Input URL address : ";
            std::cout << buffer;
            sendString(sock, buffer);

            url = readAsString(sock);
            std::cout << url << '\n';

            resolveIpAddress(url, buffer);
            std::cout << "IP address : " << buffer << '\n';
        } else if (req == Requirement::QUERY) {
            std::string sid;

            std::cout << "Handling QUERY...\n";

            buffer = "Input student ID : ";
            std::cout << buffer;
            sendString(sock, buffer);

            sid = readAsString(sock);
            std::cout << sid << '\n';

            getStudentEmail(emailDirectory, sid, buffer);

            std::cout << "Student's email : " << buffer << '\n';
        } else {
            std::cout << "Quitting...\n";
            break;
        }

        sendString(sock, buffer);
    }
}

void resolveIpAddress(std::string url, std::string &buffer)
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

void getStudentEmail(const EmailMap &emailDirectory, std::string sid,
                     std::string &buffer)
{
    EmailMap::const_iterator it;

    if ((it = emailDirectory.find(sid)) != emailDirectory.end()) {
        buffer = it->second;
    } else {
        buffer = "No such student ID";
    }
}
