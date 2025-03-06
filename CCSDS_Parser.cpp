#include <iostream>
#include <cstring> // For memcpy
#include <fstream>
#include "CCSDS_header.h" // Include the CCSDS header file
#define INCLUDE_SECONDARY_HEADER

using namespace std;

/**
 * @class CCSDS_Parser
 * @brief A class for parsing and printing details of CCSDS packets.
 * 
 * The CCSDS_Parser class is responsible for parsing CCSDS packets from raw byte buffers,
 * extracting the primary header and payload data, and displaying the parsed information in a readable format.
 * 
 * @note The class also provides the option to include a secondary header if required.
 */
class CCSDS_Parser
{
private:
public:
    /**
     * @brief Default constructor for the CCSDS_Parser class.
     */
    CCSDS_Parser() {}

    /**
     * @brief Parses a CCSDS packet from a given buffer.
     * 
     * This method extracts the primary header and payload from a given raw byte buffer and returns a 
     * dynamically allocated CCSDS_Packet structure containing both the header and the payload.
     * 
     * @param buffer A pointer to the raw data buffer containing the CCSDS packet.
     * @return A pointer to a dynamically allocated CCSDS_Packet structure containing the parsed data.
     */
    CCSDS_Packet *parseCCSDS(uint8_t *buffer)
    {
        // Just copy the primary header to get payload size
        CCSDS_PrimaryHeader primary_header;
        memcpy(&primary_header, buffer, sizeof(CCSDS_PrimaryHeader));

        // Get payload size and total size
        CCSDS_Packet *packet = (CCSDS_Packet *)malloc(sizeof(CCSDS_PrimaryHeader) + primary_header.packet_length);

        // Copy primary header
        memcpy(packet, buffer, sizeof(CCSDS_PrimaryHeader));

        // Copy payload
        memcpy(packet->data, buffer + sizeof(CCSDS_PrimaryHeader), primary_header.packet_length);

        return packet; ///< Return the parsed CCSDS packet
    }

    /**
     * @brief Prints the details of a CCSDS packet.
     * 
     * This method displays the primary header information (version, type, APID, sequence count, etc.)
     * and optionally prints the payload data based on the provided flag.
     * 
     * @param packet A pointer to the CCSDS_Packet structure containing the parsed packet.
     * @param dispPayload A boolean flag indicating whether to display the payload data (true = display, false = hide).
     */
    void printCCSDS(CCSDS_Packet *packet, bool dispPayload)
    {
        // Print given packet details
        cout << "CCSDS Packet Details:\n";
        cout << "Version: " << static_cast<int>(packet->primaryHeader.version) << "\n";
        cout << "Type: " << static_cast<int>(packet->primaryHeader.type) << "\n";
        cout << "APID: " << packet->primaryHeader.apid << "\n";
        cout << "Sequence Flags: " << static_cast<int>(packet->primaryHeader.sequence_flags) << "\n";
        cout << "Sequence Count: " << packet->primaryHeader.sequence_count << "\n";
        cout << "Packet Length: " << packet->primaryHeader.packet_length << "\n";

        // Display payload if the flag is set to true
        if (dispPayload)
        {
            cout << "Payload: ";
            for (size_t i = 0; i < packet->primaryHeader.packet_length; i++)
            {
                cout << hex << setw(2) << setfill('0') << static_cast<int>(packet->data[i]) << " ";
            }
            cout << dec << "\n"; ///< Reset to decimal format
        }
    }

    /**
     * @brief Prints the details of a CCSDS packet from a raw buffer.
     * 
     * This method directly parses a CCSDS packet from a buffer and prints its details, including
     * the primary header and optionally the payload data.
     * 
     * @param buffer A pointer to the raw data buffer containing the CCSDS packet.
     * @param bufferSize The size of the raw data buffer.
     * @param print_data A boolean flag indicating whether to print the payload data (true = display, false = hide).
     */
    void printCCSDS(const uint8_t *buffer, size_t bufferSize, bool print_data)
    {
        // Ensure the buffer is large enough to contain a valid CCSDS packet
        if (bufferSize < sizeof(CCSDS_PrimaryHeader))
        {
            std::cerr << "Error: Buffer too small to contain a valid CCSDS packet!" << std::endl;
            return;
        }

        // Create an empty packet pointer
        CCSDS_Packet *packet = (CCSDS_Packet *)malloc(sizeof(CCSDS_PrimaryHeader) + bufferSize);

        // Read the Primary Header and copy into the empty packet
        memcpy(&packet->primaryHeader, buffer, sizeof(CCSDS_PrimaryHeader));

        // Keeping track of how many bytes have been read, the offset accounts for the primary header
        size_t memory_offset = sizeof(CCSDS_PrimaryHeader);

        // Print debug
        printf("PRIMARY BUFFER OFFSET SIZE %ld\n", memory_offset);
        printf("PRIMARY HEADER INFORMATION:\n");

        cout << "Version: " << static_cast<int>(packet->primaryHeader.version) << "\n";
        cout << "Type: " << static_cast<int>(packet->primaryHeader.type) << "\n";
        cout << "Secondary Header: " << static_cast<int>(packet->primaryHeader.secondary_header) << "\n";
        cout << "APID: " << packet->primaryHeader.apid << "\n";
        cout << "Sequence Flags: " << static_cast<int>(packet->primaryHeader.sequence_flags) << "\n";
        cout << "Sequence Count: " << packet->primaryHeader.sequence_count << "\n";
        cout << "Packet Length: " << packet->primaryHeader.packet_length << "\n";

        // Print payload if the flag is set to true
        if (print_data)
        {
            size_t data_size = packet->primaryHeader.packet_length;

            // Copy buffer data into the packet data pointer
            memcpy(packet->data, buffer + memory_offset, data_size); // Copy payload data

            cout << "Payload Data: ";
            for (size_t i = 0; i < data_size; ++i)
            {
                cout << std::hex << static_cast<int>(packet->data[i]) << " ";
            }
            cout << std::dec << endl; // Return to decimal format
        }

        // Free dynamically allocated memory
        free(packet);
    }
};
