#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

int main() {
    // Bluetooth device MAC address to connect to
    const char* addr = "00:11:22:33:44:55"; // Replace with actual address

    struct sockaddr_rc loc_addr = { 0 };
    struct sockaddr_rc rem_addr = { 0 };
    int sock, status;
    
    // Allocate socket
    sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (sock < 0) {
        perror("Error opening RFCOMM socket");
        exit(1);
    }

    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_channel = (uint8_t) 1; // Channel (can vary)
    str2ba(addr, &rem_addr.rc_bdaddr);

    // Connect to the remote Bluetooth device
    status = connect(sock, (struct sockaddr *)&rem_addr, sizeof(rem_addr));
    if (status < 0) {
        perror("Error connecting to Bluetooth device");
        close(sock);
        exit(1);
    }

    std::cout << "Connected to Bluetooth device!" << std::endl;

    // Read data from Bluetooth serial port
    char buffer[1024] = {0};
    while (true) {
        int bytes_read = read(sock, buffer, sizeof(buffer));
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            std::cout << "Received data: " << buffer << std::endl;
        } else {
            std::cerr << "Error or no data received." << std::endl;
            break;
        }
    }

    close(sock);  // Close the socket
    return 0;
}
