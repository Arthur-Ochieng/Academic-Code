
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

            /* child doesn’t need the listener */

            if (send(new_fd, concatCandidateName, serverMessageLength, 0) == -1)
                perror("Server-send() error lol!");

            int valread = read(new_fd, client_message, 1024);
            printf("Client voted for Candidate No %s\n", client_message);

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