# A Simple TCP Socket Implementation
EECS3020 Introduction to Computer Networks Final Project

## Compile source code

### Option 1 : `Makefile`

Just use `make` command to compile both `client.cpp` and `server.cpp` into executables `client` and `server` as follows:

```
make
```

You can clear the executable using the following command:

```
make clean
```

### Option 2 : Compile manually

```
g++ -Wall -Wextra -pedantic server.cpp -o server
g++ -Wall -Wextra -pedantic client.cpp -o client
```

## Start the socket

### Server terminal

```
./server
```

### Client terminal

```
./client
```
