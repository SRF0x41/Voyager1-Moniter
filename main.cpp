#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "CCSDS_Parser.cpp"
#include "CCSDS_header.h"
using namespace std;

#define BUFFER_SIZE 960008 ///< Size of the buffer to hold received packet data

/**
 * @brief Main function for reading CCSDS packets from a serial port.
 * 
 * This program listens to a serial port, expects a specific byte sequence, and when the sequence is detected, 
 * it starts receiving a packet. Once a complete packet is received, it parses the packet using the CCSDS_Parser
 * and prints the details of the packet.
 * 
 * @return Returns 0 if the program runs successfully, or 1 if an error occurs.
 */
int main()
{
    const char *portName = "/dev/cu.usbmodem1101"; ///< Serial port name (change this to match your ESP32's port)

    // Open the serial port
    int serialPort = open(portName, O_RDONLY | O_NOCTTY);
    if (serialPort == -1)
    {
        cerr << "Error: Cannot open serial port!" << endl;
        return 1; ///< Return error code if serial port cannot be opened
    }

    // Configure Serial Port
    struct termios options;
    tcgetattr(serialPort, &options); ///< Get current serial port options
    cfsetispeed(&options, B115200);   ///< Set baud rate to 115200
    options.c_cflag = CS8 | CLOCAL | CREAD; // 8-bit data, local, read enabled
    options.c_iflag = IGNPAR;          ///< Ignore parity errors
    options.c_oflag = 0;               ///< No output processing
    options.c_lflag = 0;               ///< No input processing
    tcflush(serialPort, TCIFLUSH);     ///< Flush any unread data
    tcsetattr(serialPort, TCSANOW, &options); ///< Apply the serial port settings

    cout << "Listening for serial " << portName << endl;

    uint8_t byteReceived;                             ///< Variable to store the received byte
    uint8_t expectedSequence[3] = {0xAA, 0xBB, 0xCC}; ///< Expected byte sequence to trigger packet reading
    int sequenceIndex = 0;                            ///< To track the position in the expected sequence

    uint8_t packet_buffer[BUFFER_SIZE]; ///< Buffer to store incoming packet data
    int packet_bytes = 0;               ///< Variable to keep track of the number of bytes received for the packet

    while (true)
    {
        if (sequenceIndex < sizeof(expectedSequence))
        {
            // Parsing the start word
            int bytesRead = read(serialPort, &byteReceived, 1); ///< Read one byte from the serial port
            if (bytesRead > 0)
            {
                if (byteReceived == expectedSequence[sequenceIndex])
                {
                    sequenceIndex++; ///< Move to the next byte in the expected sequence
                }
            }
            else if (bytesRead < 0)
            {
                cerr << "Error: Serial read failed!" << endl;
                break; ///< Exit if serial read fails
            }
        }
        else
        {
            // Start reading packet bytes after sequence is detected
            int bytesRead = read(serialPort, packet_buffer + packet_bytes, BUFFER_SIZE - packet_bytes); ///< Read the rest of the packet
            if (bytesRead > 0)
            {
                packet_bytes += bytesRead; ///< Keep track of the total bytes received

                // Check if we have a full packet
                if (packet_bytes == BUFFER_SIZE)
                {
                    cout << "Bytes received: " << packet_bytes << endl;
                    CCSDS_Packet *packet; ///< Pointer to store the parsed CCSDS packet
                    CCSDS_Parser parser;  ///< Create CCSDS_Parser object to parse the packet

                    parser.printCCSDS(reinterpret_cast<uint8_t *>(packet_buffer), BUFFER_SIZE, false); ///< Print the parsed packet without payload

                    packet_bytes = 0;  ///< Reset byte count for the next packet
                    sequenceIndex = 0; ///< Reset sequence index for the next sequence search
                }
            }
            else if (bytesRead < 0)
            {
                cerr << "Error: Serial read failed!" << endl;
                break; ///< Exit if serial read fails
            }
        }
    }

    close(serialPort); ///< Close the serial port when done
    return 0; ///< Return 0 indicating success
}
