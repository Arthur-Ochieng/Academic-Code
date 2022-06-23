/*
Differences from the TCP server

1. Creation of a Datagram socket using SOCK_DGRAM
2. The server does not have the binding function which guarantees connection to

 the port
3. The OS finds a port each time the socket sends a packet
*/
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
    int client_fd;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];
    int server_struct_length = sizeof(server_addr);
    char *clientMessage = "Hello from client";
    char *startMessage = "Ready";


    // Creating socket file descriptor
    /*

	int server_fd = socket(domain, type,protocol);

	Domain:
    AF_INET - IPv4 Protocol
    AF_INET6 - IPv6 Protocol

	Type(Communication Type):
	SOCK_DGRAM - UDP 

	Protocol - Internet Protocol(IP) 0

	Returns -1 upon failure
    
    */
    client_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (client_fd < 0)
    {
        printf("Error while creating socket\n");
        return -1;
    }
   
    // Set port and IP:
    server_addr.sin_family = AF_INET;   //Internet Protocol - AF_NET(IPv4) or AF_INET6(IPv6)
    server_addr.sin_port = htons(2000); //Address Port
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Get input from the user:
    //printf("Enter message: ");
    //gets(client_message);

    /*
	sendto(int sockfd, void *buf, size_t len, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen)
	Returns the number of bytes from the UDP connection

	0 - indicates the end of the file
	-1 - Error 

	*/
    // Send the message to server:
    if (sendto(client_fd, startMessage, strlen(startMessage), 0,
               (struct sockaddr *)&server_addr, server_struct_length) < 0)
    {
        printf("Unable to send message\n");
        return -1;
    }
    printf("UDP Client:Message sent\n\n");


    /*
	recvfrom(int sockfd, void *buf, size_t len, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen)
	Returns the number of bytes from the UDP connection

	0 - indicates the end of the file
	-1 - Error 

	*/
    // Receive the server's response:
    if (recvfrom(client_fd, server_message, sizeof(server_message), 0,
                 (struct sockaddr *)&server_addr, &server_struct_length) < 0)
    {
        printf("Error while receiving server's msg\n");
        return -1;
    }

    printf("Msg from UDP server: %s\n", server_message);

    // Get input from the user:
    printf("> ");
    gets(client_message);

    
    /*
	sendto(int sockfd, void *buf, size_t len, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen)
	Returns the number of bytes from the UDP connection

	0 - indicates the end of the file
	-1 - Error 

	*/
    // Send the message to server:
    if (sendto(client_fd, client_message, strlen(client_message), 0,
               (struct sockaddr *)&server_addr, server_struct_length) < 0)
    {
        printf("Unable to send message\n");
        return -1;
    }
    printf("UDP Client:Message sent\n\n");


    int connectionCloseStatus = close(client_fd);
    if (connectionCloseStatus < 0)
    {
        printf("An error occured while closing the connection to the server");
        return -1;
    }
    printf("\n\nUDP Client: Connection closed\n");

    return 0;
}
