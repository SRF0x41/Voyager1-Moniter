#include <iostream>
#include <cstring>        // For memcpy
#include "CCSDS_header.h" // Include the CCSDS header file
#include "ParserCCSDS.cpp"
#define INCLUDE_SECONDARY_HEADER

using namespace std;

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

    // Free allocated memory
    free(packet);

    return 0;
}
