#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define bookmax 100
#define authormax 100
#define titlemax 100
#define publishermax 100

typedef struct {
    char title[titlemax];
    char publisher[publishermax];
    char author[authormax];
    int isbn;
    int pages;
} Book;
Book books[bookmax];
int bookcount=0;
void insert(int client_sock)
{
    if(bookcount>=bookmax)
   {write(client_sock, "Server is full, cannot add more books.\n", 40);
        return;}
Book newbook;
read(client_sock,&newbook,sizeof(newbook));
books[bookcount++]=newbook;
write(client_sock,"New book added successfully\n",29);
}
void delete(int client_sock)
{
    int isbn;
    read(client_sock,&isbn,sizeof(isbn));
    for (int i = 0; i < bookcount; i++)
    {
        if(books[i].isbn==isbn){
            for (int j = i; j < bookcount - 1; j++) {
                books[j] = books[j + 1];
            }
            bookcount--;
            write(client_sock, "Book deleted successfully.\n", 26);
            return;}
}
write(client_sock, "Book not found.\n", 17);
}
void display(int client_sock)
{
    if (bookcount == 0) {
        write(client_sock, "No books available.\n", 21);
        return;
    }

    for (int i = 0; i < bookcount; i++) {
        write(client_sock, &books[i], sizeof(Book));
    }
}
void search(int client_sock) {
    char author[authormax];
    read(client_sock, author, sizeof(author));

    int found = 0;
    for (int i = 0; i < bookcount; i++) {
        if (strcmp(books[i].author, author) == 0) {
            write(client_sock, &books[i], sizeof(Book));
            found = 1;
        }
    }

    if (!found) {
        write(client_sock, "Author not found.\n", 19);
    }
}

int main(){
int server_fd, new_sock;
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
        int ch;
        read(new_sock,&ch,sizeof(ch));
        switch(ch)
        {
            case 1: insert(new_sock);
            break;
            case 2:
            delete(new_sock);
            break;
            case 3: 
            display(new_sock);
            break;
            case 4:
            search(new_sock);
            break;
            case 5: close(new_sock);
                    printf("Client disconnected.\n");
                    exit(0);
                    break;
                default:
                    printf("Invalid option from client\n");
                    break;

        }
    }
    close(server_fd);
    return 0;
}