#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>

#define PORT 1234  // Define the port on which the server will listen
int palindrome(char * str)
{
    int k=strlen(str);
    for(int i=0; i<k/2;i++)
    {
        if(str[i]!=str[k-i-1])
        return 0;
    }
    return k;
}
int vowels(char * str)
{
    int c=0;
    int k=strlen(str);
    char vowels[]="aeiouAEIOU";
    for(int i=0;i<k;i++)
    {
for(int j=0;vowels[j]!='\0';j++)
{
if(str[i]==vowels[j])
c++;
}
    }
    return c;
}
int main() {
    int server_fd, new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char *message = "Hello, Client!"; 
    char buffer[1024]; // Message to send to the client

    // Create a TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);  // Convert to network byte order
    server_addr.sin_addr.s_addr = INADDR_ANY; // Allow any incoming address
    memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

    // Bind the socket to the address and port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        exit(1);
    }

    // Start listening for incoming connections
    if (listen(server_fd, 5) == 0) {
        printf("Server is listening on port %d...\n", PORT);
    } else {
        perror("Error in listening");
        exit(1);
    }

    addr_size = sizeof(client_addr);
    new_sock = accept(server_fd, (struct sockaddr *)&client_addr, &addr_size);
    if (new_sock < 0) {
        perror("Connection failed");
        exit(1);
    }
    int l=recv(new_sock,buffer,sizeof(buffer),0);
    buffer[l]='\0';
    int k=palindrome(buffer);
    printf("%d",k);
    send(new_sock,&k,sizeof(k),0);
    int m=vowels(buffer);
    send(new_sock,&m,sizeof(m),0);



   

    // Close the socket
    close(new_sock);
    close(server_fd);

    return 0;
}
