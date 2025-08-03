#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 1234  // Define the port on which the server will listen
int palindrome(char * str)
{
    int size=strlen(str);
    for(int i=0;i<size/2;i++)
    {
        if(str[i]!=str[size-i-1])
        return 0;
    }
    return size;
}
int countvowels(char * str)
{
    int size=strlen(str);
    int c=0;
     char vowels[] = "aeiouAEIOU";
    for(int i=0;i<size;i++)
    {
         for (int j = 0; vowels[j] != '\0'; j++) {
            if(str[i]==vowels[j])
            c++;
         }
    }
    return c;
}
int main() {
    int server_fd, new_sock;
    char buffer[1024];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char *message = "Hello, Client!"; // Message to send to the client

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
    while(1)
    {
  int bytes_received = recv(new_sock, buffer, sizeof(buffer) +1 , 0);  // Fix buffer size issue
            buffer[bytes_received] = '\0';  // Ensure null termination
if (strcmp(buffer,"Halt")==0)
{
    close(new_sock);
    close(server_fd);
    exit(0);
}
int l=palindrome(buffer);
if(l==0)
send(new_sock,&l,sizeof(l),0);
else
send(new_sock,&l,sizeof(l),0);
int k=countvowels(buffer);
send(new_sock,&k,sizeof(k),0);
    }

    // Close the socket
    close(new_sock);
    close(server_fd);

    return 0;
}
