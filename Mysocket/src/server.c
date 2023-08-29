#include <arpa/inet.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void* handleUser(void* sockPtr) {
    const int bufferSize = 1024;
    int userSocket = *((int*)sockPtr);
    free(sockPtr);

    char buffer[bufferSize];
    ssize_t recivedBytes;
    while(1) {
        recivedBytes = recv(userSocket, buffer, bufferSize, 0);
        if(recivedBytes <= 0) {
            printf("Client disconnected.\n");
            break;
        }
        buffer[recivedBytes] = '\0';

        if(strncmp(buffer, "disconnect", 10) == 0) {
            printf("Client requested disconnect.\n");
            break;
        } else if(strncmp(buffer, "shell ", 6) == 0) {
            char *command = buffer + 6;
            printf("Recived shell command: %s\n", command);
            FILE* filePtr = popen(command, "r");
            if(filePtr == NULL) {
                perror("popen");
                strcpy(buffer, "Error: Unable to execute shell command.");
            } else {
                char answer[bufferSize];
                memset(answer, 0, sizeof(answer));
                char line[bufferSize];
                size_t count = 0;
                while(fgets(line, sizeof(line), filePtr) != NULL) {
                    strcat(answer, line);
                    ++count;
                }
                if(count == 0) {
                    strcpy(buffer, "Error: Unable to execute shell command.");
                } else {
                    pclose(filePtr);
                    strcpy(buffer, answer);
                }
            }
            send(userSocket, buffer, strlen(buffer), 0);
        }
    }
    close(userSocket);
    pthread_detach(pthread_self());
    return NULL;
}

int main() {
    const int maxClients = 5;
    int serverSocket;
    int userSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_in userAddr;
    socklen_t userLen = sizeof(userAddr);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket == -1) {
        perror("socket");
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8800);

    if(bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("bind");
        return 1;
    }

    if(listen(serverSocket, maxClients) == -1) {
        perror("listen");
        return 1;
    } 

    printf("Server is listening on port 8800...\n");

    while(1) {
        userSocket = accept(serverSocket, (struct sockaddr*)&userAddr, &userLen);
        if(userSocket == -1) {
            perror("accept");
            continue;
        }
        printf("Client connected.\n");
        pthread_t tid;
        int* userSocketPtr = malloc(sizeof(int));
        *userSocketPtr = userSocket;
        if(pthread_create(&tid, NULL, handleUser, userSocketPtr) != 0) {
            perror("pthread_crate");
            close(userSocket);
        }
    }
    close(serverSocket); 
    return 0;
}
