#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
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
void insert_book(int sock) {
    Book new_book;
    printf("Enter book title: ");
    getchar(); // To clear the buffer
    fgets(new_book.title,titlemax,stdin);
    printf("Enter author name: ");
    fgets(new_book.author, authormax, stdin);
    printf("Enter accession number: ");
    scanf("%d", &new_book.isbn);
    printf("Enter total pages: ");
    scanf("%d", &new_book.pages);
    getchar(); // Clear buffer after scanf
    printf("Enter publisher: ");
    fgets(new_book.publisher, publishermax, stdin);

    write(sock, &new_book, sizeof(Book));

    char response[256];
    read(sock, response, sizeof(response));
    printf("%s", response);
}
void delete_book(int sock) {
    int accession_number;
    printf("Enter accession number of the book to delete: ");
    scanf("%d", &accession_number);
    write(sock, &accession_number, sizeof(int));

    char response[256];
    read(sock, response, sizeof(response));
    printf("%s", response);
}
void display_books(int sock) {
    Book book;
    printf("Displaying all books:\n");
    while (1)
    {
        
    
    
    int size=read(sock,&book,sizeof(Book));
    if(size==0)
    break;
    printf("\nTitle: %s", book.title);
        printf("Author: %s", book.author);
        printf("Accession Number: %d\n", book.isbn);
        printf("Total Pages: %d\n", book.pages);
        printf("Publisher: %s\n", book.publisher);
}}
void search_book(int sock) {
    char au[authormax];
    printf("Enter author name: ");
    getchar();
    fgets(au,authormax,stdin);
    write(sock,au,sizeof(au));
    char response[256];
    read(sock, response, sizeof(response));
    printf("%s", response);
}

int main() {
    int sockfd;
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

int choice;
    while (1) {
        printf("\n1. Insert new book\n2. Delete a book\n3. Display all books\n4. Search a book\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        write(sockfd, &choice, sizeof(choice));
        switch (choice) {
            case 1:
                insert_book(sockfd);
                break;
            case 2:
                delete_book(sockfd);
                break;
            case 3:
                display_books(sockfd);
                break;
            case 4:
                search_book(sockfd);
                break;
            case 5:
                close(sockfd);
                printf("Exiting...\n");
                exit(0);
                break;
            default:
                printf("Invalid option\n");
                break;
        }

        }


    // Close the socket
    close(sockfd);

    return 0;
}
