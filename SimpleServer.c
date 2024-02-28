/*
socket()
bin()
listen()
accept() == new socketfd2
recv() // recevoir message
send() // envoie message
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define IPV4 INADDR_ANY // 0x000000
#define PORT 4444
#define SIZE_BUFFER 0x400
#define FILE_LEN 0x100
#define MESSAGE_buffer_s 0x100

int main(int argc, char **argv) {

    char filename[] = "index.html";
    // int PORT2 = argv[1]; /*FEATURE CONVERT CHAR TO INT*/

    char buffer[SIZE_BUFFER] = {0};
    FILE *f = fopen(filename, "r");

    (f == NULL) ? fprintf(stderr, "Err ouverture fichier\n"),
    exit(EXIT_FAILURE) : printf("init %s", filename); 

    char buffer_t[FILE_LEN];
    char buffer_a[FILE_LEN] = " ";

    struct sockaddr_in sockAddr;

    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(PORT);
    sockAddr.sin_addr.s_addr = htonl(IPV4);

    int size_sockeaddr = sizeof(sockAddr);

    int socketFD = socket(AF_INET, SOCK_STREAM, 0);

    if(socketFD == -1) {
        fprintf(stderr, "Err creation socket\n");
        exit(EXIT_FAILURE);
    }

    int bindErr = bind(socketFD, (struct sockaddr*)&sockAddr, size_sockeaddr);

    if(bindErr == -1) {
        fprintf(stderr, "Err liaison bind au socket\n");
        exit(EXIT_FAILURE);
    }

    int listening_socket_fd = listen(socketFD, 10);

    if(listening_socket_fd == -1){
        fprintf(stderr, "Err Ecoute\nexit ..");
        exit(EXIT_FAILURE);
    }

    printf("\nEn attente d'une connexion\n");

    int acceptfd = accept(socketFD, (struct sockaddr*)&sockAddr, (socklen_t *)&size_sockeaddr);
    if(acceptfd == -1) {
        fprintf(stderr, "Err accept connexion\n");
        exit(EXIT_FAILURE);
    }

    long message = recv(acceptfd, buffer, SIZE_BUFFER, 0);
    if(message == -1){
        fprintf(stderr, "Message err\n");
        exit(EXIT_FAILURE);
    }

    while(fgets(buffer_t, sizeof(buffer_t), f) != NULL) {
        strcat(buffer_a, buffer_t);
    }

    const char message_buffer[MESSAGE_buffer_s] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";

    long sendrErr = send(acceptfd, message_buffer, sizeof(message_buffer), 0);
    long testErr = send(acceptfd, buffer_a, sizeof(buffer_a), 0);

    if(sendrErr && testErr == -1){
        fprintf(stderr, "Err sending message ..\n");
    }

    printf("Client : \n\n %s", buffer);

    close(socketFD);
    close(acceptfd);

    fclose(f);
    return 0;
}