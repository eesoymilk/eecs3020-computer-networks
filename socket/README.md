# A Simple TCP Socket Implementation

This repository hosts the codebase for a simple, yet robust, TCP socket communication program. The project was developed as a culmination of learnings from the course "**EECS3020: Introduction to Computer Networks**".

## Project Structure

The project's directory structure and file descriptions are as follows:

- `common.h`: A shared header file containing utilities and constants used across the application.

- `server.cpp`: The implementation of the server-side operations of the TCP communication application.

- `client.cpp`: The implementation of the client-side operations of the TCP communication application.

- `query.txt`: A mock database file for storing students' email information. It serves as a data source for one of the operations provided by the server.

- `Makefile`: Provides the configuration required to compile the project. Using Makefile simplifies the compilation process and ensures consistent build behaviors.

- `.clang-format`: Used to automatically format C/C++ source code for maintaining consistent code style across the project.

- `README.md` and `README.txt`: These files provide the project's documentation. `README.md` is a Markdown version for easy reading on platforms like GitHub, while `README.txt` is a plain text version for general use.

Detailed information regarding the functionality of the project is included within comments in the individual source code files.

## Compilation

You have two options for compiling the source code: using the provided `Makefile` or compiling manually.

### Option 1: Using `Makefile`

Run the following command to compile both `client.cpp` and `server.cpp` into respective executables, `client` and `server`:

```bash
make
```

To clean up the directory and remove the compiled executables, use:

```bash
make clean
```

### Option 2: Manual Compilation

If you choose to compile manually, use the following commands to compile the server and client programs:

```bash
g++ -std=c++11  -Wall -Wextra -pedantic server.cpp -o server
g++ -std=c++11  -Wall -Wextra -pedantic client.cpp -o client
```

Please note that this code must be compiled with **the C++11 standard or later** due to the usage of certain features such as **enum class**. You can use the -std flag to specify the C++ standard (e.g., `-std=c++11`, `-std=c++14`, `-std=c++17`, `-std=c++20`, or `-std=c++2b` for the latest features as of 2023).

## Usage

### Starting the Server

To start the server, execute the following command in a terminal window:

```
./server
```

The server will start and wait for incoming client connections.

### Starting the Client

To start the client, open a separate terminal window and execute the following command:

```bash
./client
```

The client program will start, establish a connection to the server, and you can then interact with the server through this client terminal.
