#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 1234  // Server port to connect to

int main() {
    int sockfd;
    int choice,num1,num2,result;
    struct sockaddr_in server_addr;
    char buffer[1024];  // Buffer to store server response

    // Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);  // Convert to network byte order
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server IP (localhost)
    memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }
    while(1)
    {
        printf("Enter your choice\n");
        scanf("%d",&choice);
        if(choice==5)
        {
            send(sockfd,&choice,sizeof(choice),0);
   close(sockfd);
   break;

        }
        printf("Enter two numbers");
        scanf("%d %d",&num1,&num2);
        send(sockfd,&num1,sizeof(num1),0);
        send(sockfd,&num2,sizeof(num1),0);
        send(sockfd,&choice,sizeof(num1),0);
        if(choice==4)
        {
            if(num2==0)
            {
                recv(sockfd, buffer, sizeof(buffer), 0);
            }
        }
        else{
        recv(sockfd,&result,sizeof(result),0);
        printf("Result: %d\n", result);}


    }

    // Receive message from the server
    

    // Close the socket
    close(sockfd);

    return 0;
}
