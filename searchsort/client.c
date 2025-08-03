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
    int odd_size, even_size;
    int ar[10], size, odd[10], even[10], ch, search;
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
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    while (1) {
        printf("\nChoose an operation:\n1. Search for a number\n2. Sort the array\n3. Split into odd and even\n4. Exit\n");
        scanf("%d", &ch);

        if (ch == 4) {
            send(sockfd, &ch, sizeof(ch), 0);  // Send choice to exit
            break;
        }

        printf("Enter the size of the array: ");
        scanf("%d", &size);

        printf("Enter the elements of the array:\n");
        for (int i = 0; i < size; i++) {
            scanf("%d", &ar[i]);
        }

        send(sockfd, &ch, sizeof(ch), 0);  // Send choice to server
        send(sockfd, &size, sizeof(size), 0);  // Send array size
        send(sockfd, ar, size * sizeof(int), 0);  // Send array

        if (ch == 1) {  // Search for a number
            printf("Enter the number to search: ");
            scanf("%d", &search);
            send(sockfd, &search, sizeof(search), 0);

            int bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);  // Fix buffer size issue
            buffer[bytes_received] = '\0';  // Ensure null termination
            printf("%s\n", buffer);
        }

        if (ch == 2) {  // Sort the array
            recv(sockfd, ar, size * sizeof(int), 0);
            printf("Sorted array: ");
            for (int i = 0; i < size; i++) {
                printf("%d ", ar[i]);
            }
            printf("\n");  // Fix missing newline
        }

        if (ch == 3) {  // Split into odd and even
            recv(sockfd, &odd_size, sizeof(odd_size), 0);
            recv(sockfd, odd, odd_size * sizeof(int), 0);
            recv(sockfd, &even_size, sizeof(even_size), 0);
            recv(sockfd, even, even_size * sizeof(int), 0);

            printf("Odd numbers: ");
            for (int i = 0; i < odd_size; i++) {
                printf("%d ", odd[i]);
            }
            printf("\n");

            printf("Even numbers: ");
            for (int i = 0; i < even_size; i++) {
                printf("%d ", even[i]);
            }
            printf("\n");
        }
    }

    // Close the socket
    close(sockfd);

    return 0;
}
