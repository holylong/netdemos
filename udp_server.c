#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
#define PORT 9080

int main(int argc, char* argv[])
{
	int sockfd;
	struct sockaddr_in server_addr, client_addr;

	socklen_t len = sizeof(client_addr);
	char buffer[BUFFER_SIZE];

    // Initialize WinSock for Windows
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "Failed to initialize WinSock.\n");
        return EXIT_FAILURE;
    }
#endif

	// create udp socket
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		perror("Error creating socket");
#ifdef _WIN32
        WSACleanup(); // Clean up WinSock
#endif
        exit(EXIT_FAILURE);
	}

	// server address setting
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	// attach socket to port
	if(bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
		perror("Error binding socket");
		close(sockfd);
#ifdef _WIN32
        WSACleanup(); // Clean up WinSock
#endif
		exit(EXIT_FAILURE);
	}

	printf("UDP server listening on port:%d\n", PORT);

	// loop for data received
	while(1){
		ssize_t bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &len);
		if(bytes_received == -1)
		{
			perror("Error receiving data");
			continue;
		}

		buffer[bytes_received] = '\0';

		fprintf(stdout, "Received %lu bytes from %s:%d\n", bytes_received, inet_ntoa(client_addr.sin_addr),
				ntohs(client_addr.sin_port));
		printf("Data: %s\n", buffer);
	}

	close(sockfd);

	return 0;
}
