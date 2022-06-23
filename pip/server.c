
#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <errno.h>

#include <string.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <sys/wait.h>

#include <signal.h>

/* the port users will be connecting to */

#define MYPORT 3000

/* how many pending connections queue will hold */

#define BACKLOG 10

void sigchld_handler(int s)

{

    while (wait(NULL) > 0)
        ;
}

int main(int argc, char *argv[])

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

    //printf(concatCandidateName);

    char server_message[100], client_message[100];

    /* listen on sock_fd, new connection on new_fd */

    int sockfd, new_fd;

    /* my address information */

    struct sockaddr_in my_addr;

    /* connector’s address information */

    struct sockaddr_in their_addr;

    int sin_size;

    struct sigaction sa;

    int yes = 1;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)

    {

        perror("Server-socket() error lol!");

        exit(1);
    }

    else

        printf("\n\nSocket has been created successfully...\n");

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)

    {

        perror("Server-setsockopt() error lol!");

        exit(1);
    }

    else

        //printf("Server-setsockopt is OK...\n");

        /* host byte order */

        my_addr.sin_family = AF_INET;

    /* short, network byte order */

    my_addr.sin_port = htons(MYPORT);

    /* automatically fill with my IP */

    my_addr.sin_addr.s_addr = INADDR_ANY;

    printf("Server is currently %s using port %d...\n", inet_ntoa(my_addr.sin_addr), MYPORT);

    /* zero the rest of the struct */

    memset(&(my_addr.sin_zero), '\0', 8);

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)

    {

        perror("Server-bind() error");

        exit(1);
    }

    else

        printf("Successfully binded the port...\n");

    if (listen(sockfd, BACKLOG) == -1)

    {

        perror("Server-listen() error");

        exit(1);
    }

    printf("Listening for client connections...\n");

    /* clean all the dead processes */

    sa.sa_handler = sigchld_handler;

    sigemptyset(&sa.sa_mask);

    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGCHLD, &sa, NULL) == -1)

    {

        perror("Server-sigaction() error");

        exit(1);
    }

    else

        //printf("Server-sigaction() is OK...\n");

        /* accept() loop */

        while (1)

        {

            sin_size = sizeof(struct sockaddr_in);
            if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1)
            {

                perror("Server-accept() error");

                continue;
            }

            else
                printf("Connection accepted...\n");

            printf("Client connected using %s\n", inet_ntoa(their_addr.sin_addr));

            /* this is the child process */

            if (!fork())

            {

                int valread = read(new_fd, client_message, 1024);

                /* child doesn’t need the listener */

                int choice = client_message - '0';
                if (choice == 1)
                {

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
                    //vote
                    if (send(new_fd, concatCandidateName, serverMessageLength, 0) == -1)
                        perror("Server-send() error lol!");
                }
                else if (choice == 2)
                {
                    //Register voter

                    // open the registerVoterMessageStream
                    FILE *registerVoterMessageStream;

                    char *registerVoterMessage;
                    size_t registerVoterMessageLength;
                    registerVoterMessageStream = open_memstream(&registerVoterMessage, &registerVoterMessageLength);

                    // write what you want with fprintf() into the registerVoterMessageStream
                    fprintf(registerVoterMessageStream, "\n\t\tMaformu Voting System\n\n\nRegister as a voter\n\n");
                    fprintf(registerVoterMessageStream, "\nPlease enter your names:\n");

                    // close the registerVoterMessageStream, the buffer is allocated and the size is set !
                    fclose(registerVoterMessageStream);

                    if (send(new_fd, registerVoterMessage, registerVoterMessageLength, 0) == -1)
                        perror("Server-send() error lol!");

                    int valread = read(new_fd, client_message, 1024);

                    char voter[50];
                    FILE *voterFile;
                    voterFile = fopen("voters.txt", "a");
                    if (voterFile == NULL)
                    {
                        printf("Error!");
                        exit(1);
                    }
                    fprintf(voterFile, "%s", client_message);
                    fclose(voterFile);
                }
                else if (choice == 3)
                {
                    //Register client as candidate

                    // open the registerCandidateMessageStream
                    FILE *registerCandidateMessageStream;

                    char *registerCandidateMessage;
                    size_t registerCandidateMessageLength;
                    registerCandidateMessageStream = open_memstream(&registerCandidateMessage, &registerCandidateMessageLength);

                    // write what you want with fprintf() into the registerCandidateMessageStream
                    fprintf(registerCandidateMessageStream, "\n\t\tMaformu Voting System\n\n\nRegister as a voter\n\n");
                    fprintf(registerCandidateMessageStream, "\nPlease enter your names:\n\t [ID No] [First Name] [Second Name] [E-mail]");

                    // close the registerCandidateMessageStream, the buffer is allocated and the size is set !
                    fclose(registerCandidateMessageStream);

                    if (send(new_fd, registerCandidateMessage, registerCandidateMessageLength, 0) == -1)
                        perror("Server-send() error lol!");

                    int valread = read(new_fd, client_message, 1024);

                    char candidate[50];
                    FILE *candidateFile;
                    candidateFile = fopen("candidates.txt", "a");
                    if (candidateFile == NULL)
                    {
                        printf("Error!");
                        exit(1);
                    }
                    fprintf(candidateFile, "%s", candidate);
                    fclose(candidateFile);

                    if (send(new_fd, registerCandidateMessage, registerCandidateMessageLength, 0) == -1)
                    {
                        perror("Server-send() error lol!");
                    }
                }

                close(new_fd);
                close(sockfd);
                exit(0);
            }

            else

                printf("Succesfully sent...\n");

            /* parent doesn’t need this*/

            close(new_fd);

            printf("Server-new socket, new_fd closed successfully...\n");
        }

    return 0;
}