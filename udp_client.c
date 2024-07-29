#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdint.h>
typedef int ssize_t;
#pragma comment(lib, "ws2_32.lib") // Link with the WinSock library
#else
#include <unistd.h>
#include <arpa/inet.h>
#endif

#define BUFFER_SIZE 1024
#define SERVER_IP "192.168.0.147"
#define SERVER_PORT 9080

int main(void) {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Initialize WinSock for Windows
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "Failed to initialize WinSock.\n");
        return EXIT_FAILURE;
    }
#endif

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Error creating socket");
#ifdef _WIN32
        WSACleanup(); // Clean up WinSock
#endif
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) == 0) {
        fprintf(stderr, "Invalid address\n");
#ifdef _WIN32
        WSACleanup(); // Clean up WinSock
#endif
        exit(EXIT_FAILURE);
    }

    while(1){
    	printf("Enter the message to send: ");
	    fgets(buffer, BUFFER_SIZE, stdin);
	    buffer[strcspn(buffer, "\n")] = '\0';

	    if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        	perror("Error sending data");
#ifdef _WIN32
            WSACleanup(); // Clean up WinSock
#endif
	        exit(EXIT_FAILURE);
    	}
    }

    printf("Data sent successfully.\n");
#ifdef _WIN32
        WSACleanup(); // Clean up WinSock
#endif
    close(sockfd);
    return 0;
}