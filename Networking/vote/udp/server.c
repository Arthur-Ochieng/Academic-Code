/*
Differences from the TCP server

1. Creation of a Datagram socket using SOCK_DGRAM
2. The server does not have the binding function which guarantees connection to the port
3. The OS finds a port each time the socket sends a packet
*/
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(void)
{

    // open the stream
    FILE *stream;

    char *concatCandidateName;
    size_t serverMessageLength;
    stream = open_memstream(&concatCandidateName, &serverMessageLength);

    printf("\n\t\tMaformu Voting System\n\n");

    char candidate_names[2][50];
    int votes[2];

    for (int i = 0; i < 2; i++)
    {
        printf("Candidate No %d > ", i);
        scanf("%s", candidate_names[i]);
    }

    // write what you want with fprintf() into the stream
    fprintf(stream, "\n\t\tMaformu Voting System\n\n\nWho are you voting for?\n\n");

    char strt[] = "";
    for (int j = 0; j < 2; j++)
    {
        sprintf(strt, "[%d] %s \n", j, candidate_names[j]);
        fprintf(stream, strt);
    }

    fprintf(stream, "\nSelect the number of your prefered candidate\n\n");

    // close the stream, the buffer is allocated and the size is set !
    fclose(stream);

    int server_fd;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];
    char *serverMessage = "Hello from server";

    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));

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
    server_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (server_fd < 0)
    {
        perror("socket failed");
        return -1;
    }

    // Set port and IP:
    server_addr.sin_family = AF_INET;         //Internet Protocol - AF_NET(IPv4) or AF_INET6(IPv6)
    server_addr.sin_port = htons(2000);       //Address Port
    server_addr.sin_addr.s_addr = INADDR_ANY; //Internet address

    //Assign address to socket:bind()
    /*
    reserves port for use

    int status = bind(socket_id,&addrport,size);

    socket_id = Socket descriptor
    addrport - struct -> IP address and port of the machine
    size - the size of the addrport structure

    Returns 
    0 - successful
    -1 - error

    */

    // Forcefully attaching socket to the port 8080
    int bindStatus = bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bindStatus < 0)
    {
        printf("Couldn't bind to the port\n");
        return -1;
    }

    printf("UDP Server:Listening for incoming messages...\n\n");

    for (;;)
    {
        struct sockaddr_in client_addr;
        int client_struct_length = sizeof(client_addr);
        memset(client_message, '\0', sizeof(client_message));

        // Receive client's message:
        if (recvfrom(server_fd, client_message, sizeof(client_message), 0,
                     (struct sockaddr *)&client_addr, &client_struct_length) < 0)
        {
            printf("Couldn't receive\n");
            return -1;
        }

        printf("Msg from UDP client: %s\n", client_message);
        int length, i;
        length = strlen(client_message);
        if (!isdigit(client_message[0]))
        {
            //printf("Entered input is not a number\n");
            // exit(1);
        }
        else
        {
            FILE *voteAppendeFile;
            voteAppendeFile = fopen("votes.txt", "a");

            if (voteAppendeFile == NULL)
            {
                printf("Error!");
                exit(1);
            }
            fprintf(voteAppendeFile, "%d", atoi(client_message));
            fclose(voteAppendeFile);

            FILE *voteFile;
            FILE *resultsFile;
            char castedVote[10000];
            voteFile = fopen("votes.txt", "r");
            fscanf(voteFile, "%s", &castedVote);
            int totalVotes;
            int results[2] = {0, 0};
            int candidateNo;
            int voteNo;

            for (candidateNo = 0; candidateNo < 2; candidateNo++)
            {
                for (totalVotes = 0; totalVotes < strlen(castedVote); totalVotes++)
                {
                    if (castedVote[totalVotes] - '0' == candidateNo)
                    {
                        results[candidateNo]++;
                    }
                }
            }
            fclose(voteFile);

            resultsFile = fopen("results.txt", "w");
            for (candidateNo = 0; candidateNo < 2; candidateNo++)
            {
                fprintf(resultsFile, "Candidate %d : %d\n", candidateNo, results[candidateNo]);
            }
            fclose(resultsFile);
        }

        if (sendto(server_fd, concatCandidateName, serverMessageLength, 0,
                   (struct sockaddr *)&client_addr, client_struct_length) < 0)
        {
            printf("An error occured while sending the message to the client");
            return -1;
        }
    }

    /* 
    Logs the Connection addresses
    
    printf("Received message from IP: %s and port: %i\n",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    */

    /*
	close(sockid) - Closes the connections
    int result = close(socket_id);
    return:
    0 - Successful
    -1 - Error
	
	*/
    close(server_fd);
    printf("\n\nUDP Server: Connection closed\n");

    return 0;
}