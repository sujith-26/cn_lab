#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctime>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

using namespace std;

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Socket creation error" << endl;
        return 1;
    }

    // Configure server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        cerr << "Invalid address/Address not supported" << endl;
        return 1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Connection failed" << endl;
        return 1;
    }

    srand(static_cast<unsigned>(time(NULL))); // Initialize random number generator

    // Receive the server's initial instruction
    recv(sock, buffer, sizeof(buffer), 0);
    buffer[BUFFER_SIZE - 1] = '\0'; // Ensure null termination
    if (strcmp(buffer, "SEND_FRAME_AND_WINDOW_SIZE") == 0) {
        long long tf, N;
        cout << "Enter total number of frames: ";
        cin >> tf;
        cout << "Enter window size: ";
        cin >> N;

        string msg = to_string(tf) + " " + to_string(N);
        send(sock, msg.c_str(), msg.size(), 0);
    }

    // Start communication
    while (true) {
        memset(buffer, 0, sizeof(buffer)); // Clear the buffer
        int valread = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (valread > 0) {
            buffer[valread] = '\0'; // Ensure null termination
            cout << buffer << endl;

            string ack;
            cout << "Enter ACK or NACK for the above frame: ";
            cin >> ack;

            send(sock, ack.c_str(), ack.size(), 0);
        } else if (valread == 0) {
            cout << "Server disconnected." << endl;
            break;
        } else {
            cerr << "Error receiving data." << endl;
            break;
        }
    }

    // Close the socket
    close(sock);
    return 0;
}
