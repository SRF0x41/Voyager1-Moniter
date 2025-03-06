# CCSDS Packet Parser for ESP32 Serial Communication

## Overview

This project is designed to parse and process CCSDS (Consultative Committee for Space Data Systems) packets received over a serial connection from an ESP32. The program listens for a specific byte sequence from the serial input, and when this sequence is detected, it reads and parses the CCSDS packet. The packet includes metadata like version, type, sequence flags, and payload data. This is similar to the communication systems used by spacecraft like Voyager 1, enabling data transmission from space exploration missions.

## How It Works

1. **Serial Port Configuration**:
   The program first establishes a serial connection to an ESP32 device, setting up parameters such as baud rate and data format.
   
2. **CCSDS Packet Structure**:
   The program defines the structure of a CCSDS packet, which consists of:
   - **Primary Header**: Contains metadata like version, packet type (telemetry or telecommand), sequence flags, and application process identifier (APID).
   - **Secondary Header (Optional)**: An additional header used for data like timestamps or data identifiers.
   - **Payload**: The actual data or message content that is transmitted in the packet.

3. **Packet Reception**:
   The program listens for an expected byte sequence (`0xAA, 0xBB, 0xCC`) which marks the beginning of a packet. Once detected, it starts reading the packet data, parsing the primary header and payload.

4. **CCSDS Parsing**:
   The program uses the `CCSDS_Parser` class to extract information from the CCSDS packet and print the details, including version, APID, sequence count, and payload data.

5. **Data Output**:
   After a full packet is received, it is parsed, and the details are printed to the console. The payload can optionally be displayed based on user preferences.

## File Structure

- **CCSDS_header.h**: Defines the structure of the CCSDS packet, including primary and secondary headers.
- **CCSDS_Parser.cpp**: Implements the `CCSDS_Parser` class for parsing and displaying packet details.
- **Main.cpp**: The main program that interfaces with the ESP32 device, receives the data, and calls the parsing functions.

## CCSDS Format

CCSDS is a widely used data format for space communications. It is designed for interoperability between different space agencies and spacecraft, including NASA, ESA, and others. The Voyager spacecraft, including Voyager 1, used a similar packetized data system for transmitting scientific and engineering data back to Earth. This code implements a simplified version of the CCSDS packet format, which can be adapted for real-time communication systems in space missions.

## How It Relates to Voyager 1

Voyager 1, launched by NASA in 1977, is a space probe that is currently the farthest human-made object from Earth. It sends back valuable scientific data about the outer solar system and interstellar space. The data sent by Voyager 1 is transmitted using a similar packetized communication format as the one implemented in this program.

### Key Features of Voyager 1's Communication:
- **Telecommand and Telemetry**: Like the packet types in this program, Voyager 1's communication system uses packets to send telemetry data (such as spacecraft status and sensor readings) and receive telecommands (instructions to the spacecraft).
- **Spacecraft Data Systems**: The CCSDS packet format used in this program mirrors the communication protocols used by Voyager 1's spacecraft data system, enabling reliable transmission of science data over vast distances.
- **Deep Space Network**: The packets transmitted by Voyager 1 are received by Earth-based antennas in NASA's Deep Space Network (DSN), which are capable of detecting signals from distant spacecraft.

By using a similar CCSDS packet format, this project demonstrates how space missions like Voyager 1 use structured data formats to ensure clear and reliable communication across vast distances in space.

## Running the Program

1. **Serial Connection**:
   - Ensure your ESP32 is connected to your computer via a USB cable.
   - Update the `portName` in the code to match the correct serial port for your ESP32 device.

2. **Compilation**:
   - Compile the program using a C++ compiler, ensuring the required libraries for serial communication (`<fcntl.h>`, `<termios.h>`, `<unistd.h>`) are available.
   
3. **Execution**:
   - Run the program, which will listen for CCSDS packets from the ESP32. The program will print out packet details, including metadata and payload data (if configured).

## Conclusion

This project serves as a simplified example of how spacecraft, such as Voyager 1, use packetized communication systems to transmit data back to Earth. The code implements a basic framework for handling CCSDS packets, which can be adapted to various space-related applications, including telemetry and telecommand systems.
