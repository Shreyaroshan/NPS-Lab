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
    int sockfd; int l,k;
    char str[1024];
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
    printf("Enter the word\n");
    scanf("%s",str);
    send(sockfd, str, sizeof(str)+1, 0);
    if (strcmp(str,"Halt")==0)
    exit(0);
    recv(sockfd,&l,sizeof(l),0);
    recv(sockfd,&k,sizeof(k),0);
    if(l==0)
    {
        printf("It is not palindrome\n");
    }
    else
    printf("It is palindrome with length %d\n",l);
    printf("Vowel Count= %d",k);



}
    // Receive message from the server
    

    // Close the socket
    close(sockfd);

    return 0;
}
