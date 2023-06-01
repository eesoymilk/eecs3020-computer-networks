# A Simple TCP Socket Implementation

This repository contains a simple implementation of **a TCP socket communication program**. It was developed as the final project for the **EECS3020 Introduction to Computer Networks** course.

## Project Structure

The project directory consists of the following files:

- `common.h`: This header file contains common utilities shared between `server.cpp` and `client.cpp`.

- `server.cpp`: This file contains the source code for the server side of the application.

- `client.cpp`: This file contains the source code for the client side of the application.

- `query.txt`: This file serves as a simple database for storing students' email information.

- `Makefile`: The `Makefile` provides the compilation configuration for the project.
    
- `.clang-format`: This file is used to auto-format the C/C++ source code for consistent code style.

- `README.md` and `README.txt`: The `README.md` file contains the project documentation in Markdown, while `README.txt` provides the same documentation in plain text format.

Please note that additional details and functionality of the project exist within the individual source code files.

## Compilation

There are two options to compile the source code: using the provided `Makefile` or compiling manually.

### Option 1: Using `Makefile`

To compile both `client.cpp` and `server.cpp` into executables named `client` and `server`, simply run the following command:

```bash
make
```

To clean the directory and remove the compiled executables, use the following command:

```bash
make clean
```

### Option 2: Manual Compilation

If you prefer to compile manually, you can use the following commands:

```bash
g++ -Wall -Wextra -pedantic server.cpp -o server
g++ -Wall -Wextra -pedantic client.cpp -o client
```

## Usage

### Server terminal

In a terminal window, execute the following command:

```
./server
```

This will start the server and listen for incoming client connections.

### Client terminal

In a separate terminal window, execute the following command:

```bash
./client
```

This will start the client and establish a connection to the server. You can use this client terminal to send and receive data through the socket.
