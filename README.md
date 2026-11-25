# UCYSS Evil Twin Demonstration

An ESP32-based **Evil Twin Wi-Fi Simulation** developed for educational cybersecurity awareness and demonstration purposes by UCYSS.

The project demonstrates how attackers can imitate legitimate public Wi-Fi networks and how users can be exposed to fake captive portals.

## Features

- Creates a Wi-Fi Access Point using ESP32.
- Simulates a Starbucks-style free Wi-Fi captive portal.
- Automatically redirects connected devices to the demonstration page.
- Displays submitted demo credentials in the Arduino Serial Monitor.
- Shows an educational awareness page after submission.
- Includes UCYSS social media links.
- Supports Android, Apple, and Windows captive portal detection.

## Hardware Requirements

- ESP32 Development Board
- USB Cable
- Computer with Arduino IDE

## Software Requirements

- Arduino IDE
- ESP32 Board Package
- Required libraries:
  - `WiFi.h`
  - `DNSServer.h`
  - `WebServer.h`
  - `esp_wifi.h`

## Configuration

| Setting | Value |
|---|---|
| SSID | Starbucks Wifi |
| IP Address | 192.168.4.1 |
| Wi-Fi Channel | 6 |
| Security | Open Network |
| Maximum Clients | 4 |
| Serial Baud Rate | 115200 |

## Installation & Usage

1. Clone or download this repository.
2. Open the `.ino` file using Arduino IDE.
3. Connect your ESP32 to your computer.
4. Select the correct ESP32 board and COM port.
5. Upload the code.
6. Open Serial Monitor and set the baud rate to `115200`.
7. Connect to the demonstration Wi-Fi network.
8. Open the captive portal when prompted, or visit `http://192.168.4.1`.

## Demonstration Flow

1. ESP32 broadcasts the demonstration Wi-Fi network.
2. A participant connects to the network.
3. The Starbucks-style demonstration portal appears.
4. The participant submits demonstration-only credentials.
5. The submitted values are displayed in the ESP32 Serial Monitor.
6. The educational reveal page explains the Evil Twin attack concept.

## Disclaimer

**For educational and authorized cybersecurity awareness demonstrations only.**

This project is designed to demonstrate the risks of connecting to fake public Wi-Fi networks.

- Use only in a controlled environment.
- Obtain informed consent from all participants.
- Never collect real usernames or passwords.
- Do not imitate public Wi-Fi networks in public spaces without explicit authorization.
- Do not use this project for unauthorized access, credential theft, or malicious activities.

---

**Developed by UCYSS – Cybersecurity Student Association, UPTM.**
