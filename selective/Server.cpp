#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

#define PORT 8080
#define ll long long int

using namespace std;

void transmission(int client_socket, ll tf, ll N) {
    vector<bool> received(tf, false);
    ll i = 0;
    char buffer[1024] = {0};

    while (i < tf) {
        for (int k = i; k < i + N && k < tf; k++) {
            memset(buffer, 0, sizeof(buffer));
            int valread = recv(client_socket, buffer, 1024, 0);
            if (valread > 0) {
                cout << "Received: " << buffer << endl;
                string ack = "ACK" + to_string(k + 1);
                send(client_socket, ack.c_str(), ack.size(), 0);
                received[k] = true;
            }
        }
        for (int k = i; k < i + N && k < tf; k++) {
            if (received[k]) {
                cout << "Acknowledgment for Frame " << k + 1 << " sent." << endl;
            }
        }
        while (i < tf && received[i]) {
            i++;
        }
    }
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    cout << "Waiting for client connection..." << endl;
    if ((client_socket = accept(server_fd, (struct sockaddr*)&address, 
                                (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    cout << "Client connected." << endl;
    ll tf, N;
    char buffer[1024] = {0};

    cout << "Requesting frame count and window size from client..." << endl;
    recv(client_socket, buffer, 1024, 0);
    sscanf(buffer, "%lld %lld", &tf, &N);

    cout << "Total Frames: " << tf << ", Window Size: " << N << endl;
    transmission(client_socket, tf, N);

    close(client_socket);
    close(server_fd);
    return 0;
}
