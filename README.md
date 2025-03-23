# 📡 Ade' GNSS Tracker

A multithreaded C application that connects to a GNSS receiver (e.g., Quectel LC29HBA), logs raw NMEA messages, and parses `$GNGGA` messages to extract fix-related data such as latitude, longitude, altitude, satellite count, and more.

> Designed for high-resolution tracking and logging with command control, error resilience, and flexible runtime options.

---

## 🚀 Features

- 📡 Connects to a GNSS receiver via serial port
- 📄 Logs raw NMEA messages to timestamped log files
- 📍 Parses `$GNGGA` messages for structured GNSS fix data
- 🔁 Multithreaded:
  - Serial data logger
  - GNSS fix parser
  - GNSS command interface
- 🛠️ Command handling using Quectel PQTM protocol
- 🧼 Clean exit and resource management
- 💾 Organized logs and results

---

## 📁 Project Structure

```
.
├── gnsstracker.c             # Main entry point
├── gnsstracker.h             # Header file with shared config and declarations
├── init.c                    # Initialization of log files and startup commands
├── serial.c                  # Serial port setup and raw data logging
├── process_nmea_data.c       # GGA message parsing and result logging
├── handle_commands.c         # PQTM command interface (start, stop, reset, etc.)
├── error.c                   # Error code printing and cleanup routine
├── logs/                     # Generated logs (created automatically)
```

---

## ⚙️ Configuration

Edit `gnsstracker.h` to match your device settings:

```c
#define PORT "/dev/ttyUSB0"   // Or your GNSS serial port
#define BAUD_RATE B115200     // Default for LC29HBA
#define DURATION 5            // Logging duration in seconds (0 = run for maximum of 1 day)
```

---

## 🛠️ Build Instructions

### Requirements
- POSIX-compliant OS (Linux/macOS or Raspberry Pi)
- C compiler (GCC or Clang)
- GNSS receiver (e.g., Quectel LC29HBA)
- Serial port access

### Compile
```bash
gcc gnsstracker.c init.c serial.c process_nmea_data.c handle_commands.c error.c -o gnsstracker -lpthread
```

---

## ▶️ Running the Tracker

```bash
sudo ./gnsstracker
```

By default:
- Logs raw GNSS data for `DURATION` seconds (set in header)
- Stores output files under the `logs/` directory

---

## 📄 Output Files

All logs are auto-timestamped and stored in `logs/`:

- `gnss_logs_YYYYMMDD_HHMMSS.txt` – raw NMEA messages
- `gnss_results_YYYYMMDD_HHMMSS.txt` – parsed fix data from `$GNGGA`
- `gnss_commands_YYYYMMDD_HHMMSS.txt` – responses from PQTM commands

---

## 🛰️ Parsed GNSS Fix Fields (`$GNGGA`)

- UTC Time
- Latitude / Longitude
- Fix Quality (0 = Invalid, 1 = GNSS, 2 = DGNSS)
- Number of Satellites
- HDOP (Horizontal Dilution of Precision)
- Altitude (meters)
- Geoid Height

---

## 📦 Supported PQTM Commands

Automatically sent during startup and customizable via `handle_commands.c`.

| Code | Description         |
| ---- | ------------------- |
| `V`  | Get version info    |
| `G`  | Start GNSS engine   |
| `T`  | Stop GNSS engine    |
| `C`  | Cold start          |
| `W`  | Warm start          |
| `H`  | Hot start           |
| `R`  | Restart GNSS engine |

---

## 🧼 Clean Exit Behavior

The program registers a cleanup routine using `atexit()`:
- Sends stop command to GNSS
- Closes serial port and all files
- Flushes buffers
- Displays exit message

---

## 📌 Notes

- Assumes NMEA 0183 messages are output from GNSS module.
- Currently parses only `$GNGGA`. You can extend it to support `$GNRMC`, `$GNVTG`, etc.
- Set `DURATION = 0` for maximum of 1 day tracking.
- GNSS commands are issued at startup via `init()`.

---

## 🧪 Example Output (Parsed Fix)

```
UTC Time: 123519.000
Latitude: 3723.2475 N
Longitude: 12158.3416 W
Fix Quality: 1
Number of Satellites: 08
HDOP: 0.9
Altitude: 545.4 M
Geoid Height: 46.9 M
```

---

## 📄 License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT).
Use it freely and modify as needed.
