#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "CCSDS_header.h"
#include "CCSDS_Parser.cpp"

using namespace std;
#define BUFFER_SIZE 960008  // Fixed chunk size

int main() {
    const char* portName = "/dev/cu.usbmodem1101";  // Update to match your ESP32's serial port

    // Open the serial port
    int serialPort = open(portName, O_RDONLY | O_NOCTTY);
    if (serialPort == -1) {
        cerr << "Error: Cannot open serial port!" << endl;
        return 1;
    }

    // Configure Serial Port
    struct termios options;
    tcgetattr(serialPort, &options);
    cfsetispeed(&options, B115200);  // Set baud rate to 115200
    options.c_cflag = CS8 | CLOCAL | CREAD;  // 8-bit data, local, read enabled
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(serialPort, TCIFLUSH);
    tcsetattr(serialPort, TCSANOW, &options);

    cout << "Listening for serial data..." << endl;

    char buffer[BUFFER_SIZE];  // Fixed buffer size
    int bytesReceived = 0;     // Tracks the number of bytes received

    while (true) {
        // Read data in chunks until we accumulate BUFFER_SIZE bytes
        int bytesRead = read(serialPort, buffer + bytesReceived, BUFFER_SIZE - bytesReceived);

        if (bytesRead > 0) {
            bytesReceived += bytesRead;  // Keep track of total bytes received

            // Check if we have a full packet
            if (bytesReceived == BUFFER_SIZE) {
                cout << "Bytes recieved " << bytesReceived << endl;
                CCSDS_Packet *packet;
                CCSDS_Parser parser;

                parser.printCCSDS(reinterpret_cast<uint8_t*>(buffer),BUFFER_SIZE,false);

                free(packet);
                bytesReceived = 0;
            }
        } else if (bytesRead < 0) {
            cerr << "Error: Serial read failed!" << endl;
            break;
        }
    }

    close(serialPort);
    return 0;
}
