# LoRaWAN to NRF-Radio Text Message Relay

- This project is a text message relay system that connects LoRaWAN and NRF radio modules using Node.js and Express.js. 
- It enables data transmission between two separate networks, allowing messages to be relayed between LoRaWAN and NRF devices. 
- The system could only support 2 users and lacked features like logical addressing and other networking features

Below is an overview of the project structure and its key components.

## Project Structure

The project is organized into several directories, each with its own functionality and set of files.

### LoraWAN_WiFi_Adapter

- `LoraWAN_WiFi_Adapter.ino`: The Arduino sketch that runs on an ESP32 CAM module acting as a LoRaWAN to WiFi adapter.
- `net.h`: Header file containing WiFi configurations functions.
- `radio.h`: Header file for radio communication functions.
- `server.h`: Header file for defining server routes and operations.

This component serves as a LoRaWAN to WiFi adapter, allowing wireless devices to bridge to a local network.

### MessageRouter

- `relay.js`: JavaScript file defining relay operations. Basically its the main program.
- `start.sh`: A bash script for starting the relay server.
- `run.sh`: A bash script for configuring the relay system.
- `drivers` directory: Contains specific Javascript 'drivers' for the radio interfaces on the relay computer.

This part of the project serves as the core message router, managing communication between NRF and LoRaWAN radio networks.

### MessageRouter - config

- Configuration commands: Sample commands for configuring interfaces and bridging them.

This directory provides sample configuration commands to create, bridge, and configure interfaces between NRF and LoRaWAN devices.

### MessageTerminalApp

- Code for an Android app which the two clients use to exchanges text messages

### NRF_Serial_Adapter

- Header file with NRF-related command definitions.
- C++ source file with NRF communication operations.

This component manages the NRF radio modules, including configuring channels, options, and sending/receiving data.

### NRF_WiFi_Adapter

- C++ source file with server initialization and routes for NRF to WiFi adaptation.


### Relay Radio Interface Driver

- JavaScript files with functions for creating, bridging, writing, and destroying radio devices.

This component is essential for managing the radio devices and relay operations.

## How the Project Works

The project serves as a bridge between two networks, LoRaWAN and NRF. 
The LoRaWAN to WiFi adapter allows wireless devices to connect to the local network since some phones and devices dont have Lora or NRF hardware. 
The MessageRouter facilitates communication between NRF and LoRaWAN devices, relaying text messages between them.

## Note
- Documentation for this project is still in progress
