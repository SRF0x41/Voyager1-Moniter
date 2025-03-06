/**
 * @file CCSDS_header.h
 * @brief Defines the structure of the CCSDS (Consultative Committee for Space Data Systems) packet format.
 * 
 * This header file defines the structures for the CCSDS Primary Header, Secondary Header (optional), 
 * and the full CCSDS Packet. These structures are used to parse and process CCSDS packets, commonly used 
 * in space applications.
 * 
 * @note Ensure that the correct alignment is applied using `#pragma pack(push, 1)` and `#pragma pack(pop)`.
 */

#ifndef CCSDS_HEADER  ///< Header guard to prevent multiple inclusions
#define CCSDS_HEADER

#pragma pack(push, 1) ///< Ensure no padding in structure fields

/**
 * @brief CCSDS Primary Header structure.
 * 
 * This structure represents the primary header of a CCSDS packet, which contains metadata such as 
 * version, type, APID (Application Process Identifier), sequence flags, and packet length.
 * 
 * @note The packet_length field stores the length of the payload in the packet.
 */
typedef struct
{
    uint8_t version : 3;          ///< 3-bit version (0-7)
    uint8_t type : 1;             ///< 1-bit type (0 = telemetry, 1 = telecommand)
    uint8_t secondary_header : 1; ///< 1-bit secondary header flag (0 = no, 1 = yes)
    uint16_t apid : 11;           ///< 11-bit APID (Application Process Identifier)
    uint8_t sequence_flags : 2;   ///< 2-bit Sequence Flags
    uint16_t sequence_count : 14; ///< 14-bit Sequence Count
    uint32_t packet_length;       ///< 16-bit Packet Length
} CCSDS_PrimaryHeader;

#pragma pack(pop) ///< Restore default alignment

#pragma pack(push, 1) 

/**
 * @brief CCSDS Secondary Header structure (optional).
 * 
 * This structure holds additional metadata, such as a timestamp and data identifier, and is included
 * when the secondary header flag in the primary header is set.
 */
typedef struct
{
    uint32_t timestamp; ///< 32-bit timestamp, often used to store time in a specific format (e.g., Unix epoch)
    uint16_t data_id;   ///< Data identifier for the specific data type
} CCSDS_SecondaryHeader;

#pragma pack(pop) 

#pragma pack(push, 1) 

/**
 * @brief CCSDS Packet structure.
 * 
 * The CCSDS_Packet structure combines the primary header, an optional secondary header, and the variable-length payload data.
 * 
 * @note The `data[]` array is a flexible array member that holds the packet's payload.
 */
typedef struct
{
    CCSDS_PrimaryHeader primaryHeader;    ///< Primary header of the CCSDS packet
#ifdef INCLUDE_SECONDARY_HEADER
    CCSDS_SecondaryHeader secondaryHeader; ///< Optional secondary header if enabled
#endif
    uint8_t data[]; ///< Payload data (variable length)
} CCSDS_Packet;

#pragma pack(pop) 

#endif // CCSDS_HEADER
