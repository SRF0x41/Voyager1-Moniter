#ifndef CCSDS_header  // Header guard start
#define CCSDS_header
#include <iostream>

/*Structs*/

#pragma pack(push, 1) // Ensure no padding is added to the struct

typedef struct
{
  uint8_t version;          // 3 bits version
  uint8_t type;             // 1 bit type 0 for telemetry 1 for telecommand
  uint8_t secondary_header; // 1 bit for true false
  uint8_t apid;             // [8 bits APID LSB]
  uint8_t sequence_flags;   // [2 bits Sequence Flags] | [14 bits Sequence Count MSB]
  uint8_t sequence_count;   // [8 bits Sequence Count LSB]
  uint16_t packet_length;   // [16 bits Packet Length] (Payload size - 1)
} CCSDS_PrimaryHeader;

#pragma pack(pop) // Restore default struct padding

#pragma pack(push, 1)
typedef struct
{
  uint32_t timestamp; // Example: 32-bit timestamp
  uint16_t data_id;   // Identifier for specific data type
} CCSDS_SecondaryHeader;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
  CCSDS_PrimaryHeader primaryHeader;
#ifdef INCLUDE_SECONDARY_HEADER
  CCSDS_SecondaryHeader secondaryHeader; // Optional
#endif
  uint8_t data[]; // Payload (variable length)
} CCSDS_Packet;
#pragma pack(pop)



#endif // GENERIC_H  // Header guard end
