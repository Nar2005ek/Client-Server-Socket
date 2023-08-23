#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    const int maxBuffer = 1024;
    int userSocket = -1;
    struct sockaddr_in serverAddr = {0};

    char buffer[maxBuffer];
    char serverIP[30];
    int serverPort = 0;

    while(1) {
        printf("Client> ");r
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strlen(buffer) - 1] = '\0';

        if(strncmp(buffer, "disconnect", 10) == 0) {
            if(userSocket != -1) {
                send(userSocket, buffer, strlen(buffer), 0);
                printf("Disconnected from server.\n");
                close(userSocket);
                userSocket = -1;
            } else {
                printf("Not connected to any server.\n");
            }
        } else if(strncmp(buffer, "shell ", 6) == 0) {
            if(userSocket != -1) {
                send(userSocket, buffer, strlen(buffer), 0);
                ssize_t recivedBytes = recv(userSocket, buffer, sizeof(buffer), 0);
                if(recivedBytes > 0) {
                    buffer[recivedBytes] = '\0';
                    if(strncmp(buffer, "Error", 5) == 0) {
                        printf("Server error: %s\n", buffer);
                    } else {
                        printf("Server answer: \n%s", buffer);
                    }
                
            } else {
                printf("Not connected to any server.\n");
            }
        } else if(strncmp(buffer, "connect", 7) == 0) {
            if(userSocket == -1) {
                sscanf(buffer + 8, "%s %d", serverIP, &serverPort);
                userSocket = socket(AF_INET, SOCK_STREAM, 0);
                if(userSocket == -1) {
                    perror("Socket");
                    return 1;
                }
                serverAddr.sin_family = AF_INET;
                serverAddr.sin_addr.s_addr = inet_addr(serverIP);
                serverAddr.sin_port = htons(serverPort);

                if(connect(userSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
                    perror("Connect");
                    close(userSocket);
                    userSocket = -1;
                } else {
                    printf("Connected to server at %s.%d:\n", serverIP, serverPort);
                }
            } else {
                printf("Already connected to a server:\n");
            }
        } 
        else {
            printf("Invalid input. Please enter 'connect', 'disconnect' or a valid  shell command:\n");
        }
    }
    if(userSocket != -1) {
        close(userSocket);
    }
    return 0;
}