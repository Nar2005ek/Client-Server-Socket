# Basic Client-Server Communication in C

This repository contains simple client and server programs implemented in C that demonstrate basic socket communication. The client can connect to the server and send shell commands, and the server can execute these commands and send back the output to the client.

## Features

- Allows clients to connect to the server and send shell commands.
- Server executes received shell commands and sends back the output to clients.
- Supports handling multiple clients using threads.
- Provides a basic command-line interface for the client to interact with the server.

## Prerequisites

- GCC (GNU Compiler Collection) is required to compile the programs. If you don't have GCC installed, you can download it from [here](https://gcc.gnu.org/).

## Compilation and Usage

1. Clone the repository:

   ```bash
   git clone https://github.com/yourusername/ClientServerCommunication.git
   cd ClientServerCommunication
