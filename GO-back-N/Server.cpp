#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <ctime>

#define PORT 8080
#define ll long long int

using namespace std;

void transmission(int client_socket, ll tf, ll N) {
    ll total_transmissions = 0; // Total frames sent including retransmissions
    ll i = 1; // Current frame index

    while (i <= tf) {
        int successful_acks = 0; // Number of acknowledged frames in the current window

        // Sending frames in the current window
        for (int k = i; k < i + N && k <= tf; k++) {
            string msg = "Sending Frame " + to_string(k);
            send(client_socket, msg.c_str(), msg.size(), 0);
            cout << msg << endl;
            total_transmissions++;
            usleep(500000); // Delay for visualization (500ms)
        }

        // Receiving ACKs for the frames
        for (int k = i; k < i + N && k <= tf; k++) {
            char buffer[1024] = {0};
            fd_set readfds;
            struct timeval tv;

            // Initialize the file descriptor set
            FD_ZERO(&readfds);
            FD_SET(client_socket, &readfds);

            // Set timeout for acknowledgment
            tv.tv_sec = 8;
            tv.tv_usec = 0;

            // Check for activity on the socket
            int activity = select(client_socket + 1, &readfds, NULL, NULL, &tv);

            if (activity > 0) {
                int valread = recv(client_socket, buffer, sizeof(buffer), 0);
                if (valread > 0) {
                    buffer[valread] = '\0'; // Ensure buffer is null-terminated
                    if (strcmp(buffer, "ACK") == 0) {
                        cout << "Acknowledgment received for Frame " << k << endl;
                        successful_acks++;
                    } else {
                        cout << "NACK received for Frame " << k << endl;
                        cout << "Retransmitting Window..." << endl;
                        break;
                    }
                } else {
                    cout << "Error receiving ACK for Frame " << k << endl;
                    cout << "Retransmitting Window..." << endl;
                    break;
                }
            } else {
                cout << "Timeout! Frame Number " << k << " Not Received" << endl;
                cout << "Retransmitting Window..." << endl;
                break;
            }
        }

        // Update the starting frame index for the next window
        i += successful_acks;
        cout << "\n";
    }

    cout << "Total frames sent and resent: " << total_transmissions << endl;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);

    // Create server socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Socket creation failed." << endl;
        return 1;
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "Bind failed." << endl;
        close(server_socket);
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_socket, 3) < 0) {
        cerr << "Listen failed." << endl;
        close(server_socket);
        return 1;
    }

    cout << "Server is listening on port " << PORT << "..." << endl;

    // Accept a client connection
    if ((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addrlen)) < 0) {
        cerr << "Failed to accept connection." << endl;
        close(server_socket);
        return 1;
    }

    cout << "Connection established with client." << endl;

    // Simulate the transmission process
    ll total_frames, window_size;
    char buffer[1024] = {0};

    // Receive total frames and window size from client
    recv(client_socket, buffer, sizeof(buffer), 0);
    sscanf(buffer, "%lld %lld", &total_frames, &window_size);
    cout << "Total Frames: " << total_frames << ", Window Size: " << window_size << endl;

    // Start transmission
    transmission(client_socket, total_frames, window_size);

    // Cleanup
    close(client_socket);
    close(server_socket);

    return 0;
}
