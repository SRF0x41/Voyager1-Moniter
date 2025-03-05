#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "CCSDS_Parser.cpp"
#include "CCSDS_header.h"
using namespace std;

#define BUFFER_SIZE 960008

int main()
{
    const char *portName = "/dev/cu.usbmodem1101"; // Update to match your ESP32's serial port

    // Open the serial port
    int serialPort = open(portName, O_RDONLY | O_NOCTTY);
    if (serialPort == -1)
    {
        cerr << "Error: Cannot open serial port!" << endl;
        return 1;
    }

    // Configure Serial Port
    struct termios options;
    tcgetattr(serialPort, &options);
    cfsetispeed(&options, B115200);         // Set baud rate to 115200
    options.c_cflag = CS8 | CLOCAL | CREAD; // 8-bit data, local, read enabled
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(serialPort, TCIFLUSH);
    tcsetattr(serialPort, TCSANOW, &options);

    cout << "Listening for serial " << portName << endl;

    uint8_t byteReceived;                             // Variable to store the received byte
    uint8_t expectedSequence[3] = {0xAA, 0xBB, 0xCC}; // Expected byte sequence
    int sequenceIndex = 0;                            // To track the position in the sequence

    uint8_t packet_buffer[BUFFER_SIZE];
    int packet_bytes = 0;

    while (true)
    {
        if (sequenceIndex < sizeof(expectedSequence))
        {
            // Parsing the start word
            int bytesRead = read(serialPort, &byteReceived, 1);
            if (bytesRead > 0)
            {
                if (byteReceived == expectedSequence[sequenceIndex])
                {
                    sequenceIndex++;
                }
            }
            else if (bytesRead < 0)
            {
                cerr << "Error: Serial read failed!" << endl;
                break;
            }
        }
        else
        {
            // Start word read start reading packet bytes;
            int bytesRead = read(serialPort, packet_buffer + packet_bytes, BUFFER_SIZE - packet_bytes);
            if (bytesRead > 0)
            {
                packet_bytes += bytesRead; // Keep track of total bytes received

                // Check if we have a full packet
                if (packet_bytes == BUFFER_SIZE)
                {
                    cout << "Bytes recieved " << packet_bytes << endl;
                    CCSDS_Packet *packet;
                    CCSDS_Parser parser;

                    parser.printCCSDS(reinterpret_cast<uint8_t *>(packet_buffer), BUFFER_SIZE, false);

                    free(packet);
                    packet_bytes = 0;
                    sequenceIndex = 0;
                }
            }
            else if (bytesRead < 0)
            {
                cerr << "Error: Serial read failed!" << endl;
                break;
            }
        }
    }

    close(serialPort);
    return 0;
}
