#include <iostream>
#include <cstring>        // For memcpy
#include "CCSDS_header.h" // Include the CCSDS header file
#define INCLUDE_SECONDARY_HEADER

using namespace std;

void printCCSDSPacket(const uint8_t *buffer, size_t bufferSize)
{
    if (bufferSize < sizeof(CCSDS_PrimaryHeader))
    {
        std::cerr << "Error: Buffer too small to contain a valid CCSDS packet!" << std::endl;
        return;
    }

    // Create an empty packet pointer
    CCSDS_Packet *packet = (CCSDS_Packet*)malloc(sizeof(CCSDS_PrimaryHeader) + bufferSize);

    // Read the Primary Header and copy into the empty packet
    memcpy(&packet->primaryHeader, buffer, sizeof(CCSDS_PrimaryHeader));

    // Keeping track of how many bytes have been read, the offset accounts for the primary header
    size_t memory_offset = sizeof(CCSDS_PrimaryHeader);

    // Print debug
    printf("PRIMARY BUFFER OFFSET SIZE %ld\n", memory_offset);
    printf("PRIMARY HEADER INFORMATION:\n");

    cout << "version: " << static_cast<int>(packet->primaryHeader.version) << "\n";
    cout << "type: " << static_cast<int>(packet->primaryHeader.type) << "\n";
    cout << "secondary_header: " << static_cast<int>(packet->primaryHeader.secondary_header) << "\n";
    cout << "apid: " << packet->primaryHeader.apid << "\n";
    cout << "sequence_flags: " << static_cast<int>(packet->primaryHeader.sequence_flags) << "\n";
    cout << "sequence_count: " << packet->primaryHeader.sequence_count << "\n";
    cout << "packet_length: " << packet->primaryHeader.packet_length << "\n";

    // Print debug
    size_t data_size = packet->primaryHeader.packet_length;

    // Copy buffer data into the packet data pointer
    memcpy(packet->data, buffer + memory_offset, data_size);  // Copy payload data

    // Debug print for the data (optional)
    cout << "Payload Data: ";
    for (size_t i = 0; i < data_size; ++i)
    {
        cout << std::hex << static_cast<int>(packet->data[i]) << " ";
    }
    cout << std::dec << endl; // Return to decimal format

    // Free allocated memory
    free(packet);
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
    printCCSDSPacket(buffer, packetSize);

    // Free allocated memory
    free(packet);

    return 0;
}
