#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

using namespace std;

int main(){
    return 0;
}

/*
int main() {
    const char* portName = "/dev/tty.usbserial-0001";  // Change this to your ESP32's serial port

    int serialPort = open(portName, O_RDONLY | O_NOCTTY);
    if (serialPort == -1) {
        cerr << "Error: Cannot open serial port!" << endl;
        return 1;
    }

    // Configure Serial Port
    struct termios options;
    tcgetattr(serialPort, &options);
    cfsetispeed(&options, B115200);  // Set baud rate to match ESP32
    options.c_cflag = CS8 | CLOCAL | CREAD; // 8-bit data
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(serialPort, TCIFLUSH);
    tcsetattr(serialPort, TCSANOW, &options);

    cout << "Listening for serial data..." << endl;
    ofstream binFile("Tranmission.bin", ios::binary);

    if (!binFile) {
        cerr << "Error: Unable to create binary file!" << endl;
        return 1;
    }
    //
    char buffer[256];
    while (true) {
        int bytesRead = read(serialPort, buffer, sizeof(buffer));
        if (bytesRead > 0) {
            binFile.write(buffer, bytesRead);
            cout << "Received " << bytesRead << " bytes and saved to file." << endl;
        }
    }

    binFile.close();
    close(serialPort);
    return 0;
}


int main()
{
    size_t payloadSize = 3; // Example: 3-byte payload
    size_t packetSize = sizeof(CCSDS_PrimaryHeader) + payloadSize;

    // Allocate memory dynamically for the CCSDS_Packet with enough space for data[]
    CCSDS_Packet *packet = (CCSDS_Packet *)malloc(sizeof(CCSDS_PrimaryHeader) + payloadSize);
    if (!packet)
    {
        std::cerr << "Memory allocation failed!" << std::endl;
        return -1;
    }

    // Initialize Primary Header
    packet->primaryHeader.version = 0b001;
    packet->primaryHeader.type = 0;
    packet->primaryHeader.secondary_header = 0;
    packet->primaryHeader.apid = 100;
    packet->primaryHeader.sequence_flags = 3;
    packet->primaryHeader.sequence_count = 42;
    packet->primaryHeader.packet_length = payloadSize;

    // Example Payload Data
    uint8_t exampleData[] = {0xAB, 0xCD, 0xEF};
    memcpy(packet->data, exampleData, payloadSize);

    // Serialize Packet into a Byte Array
    uint8_t buffer[packetSize];
    memcpy(buffer, packet, packetSize);

    // Read Back the Packet
    cout << "Reading CCSDS Packet..." << std::endl;

    ParserCCSDS parser;
    parser.printCCSDSPacket(buffer, packetSize, true);

    // Writing to transmission.bin
    cout << "Writing to Transmission.bin" << "\n";
    parser.writeTransmission(packet);

    // Readint transmission.bin
    parser.readTransmission();

    // Free allocated memory
    free(packet);

    return 0;
}
*/