#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For close()
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 1234  // Define the port on which the server will listen

int main() {
    int server_fd, new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    int odd_size, even_size;
    int flag = 0;
    int ar[10], size, odd[10], even[10], ch, search, swap;
    int k = 0, l = 0;
    char *message1 = "Search successful!";
    char *message2 = "Search not successful!";

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

    while (1) {
        recv(new_sock, &ch, sizeof(ch), 0);
        if (ch == 4) {
            close(new_sock);
            close(server_fd);
            exit(0);
        }

        recv(new_sock, &size, sizeof(size), 0);
        recv(new_sock, ar, size * sizeof(int), 0);

        switch (ch) {
            case 1:  // Search for a number
                recv(new_sock, &search, sizeof(search), 0);
                flag = 0;
                for (int i = 0; i < size; i++) {
                    if (ar[i] == search) {
                        send(new_sock, message1, strlen(message1) + 1, 0);  // Include null terminator
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0)
                    send(new_sock, message2, strlen(message2) + 1, 0);  // Include null terminator
                break;

            case 2:  // Sort the array (Bubble Sort)
                for (int i = 0; i < size - 1; i++) {
                    for (int j = 0; j < size - i - 1; j++) {  // Fixed incorrect loop
                        if (ar[j] > ar[j + 1]) {
                            swap = ar[j];
                            ar[j] = ar[j + 1];
                            ar[j + 1] = swap;
                        }
                    }
                }
                send(new_sock, ar, size * sizeof(int), 0);
                break;

            case 3:  // Split into odd and even
                k = 0;
                l = 0;
                for (int i = 0; i < size; i++) {
                    if (ar[i] % 2 == 0) {
                        even[k++] = ar[i];
                    } else {
                        odd[l++] = ar[i];
                    }
                }
                even_size = k;
                odd_size = l;

                send(new_sock, &odd_size, sizeof(odd_size), 0);
                send(new_sock, odd, odd_size * sizeof(int), 0);

                send(new_sock, &even_size, sizeof(even_size), 0);
                send(new_sock, even, even_size * sizeof(int), 0);
                break;
        }
    }

    // Close the socket
    close(new_sock);
    close(server_fd);

    return 0;
}
