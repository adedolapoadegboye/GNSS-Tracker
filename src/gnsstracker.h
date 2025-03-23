#ifndef gnsstracker
#define gnsstracker

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>

// Initialize Program
void init();
char filename[64];
FILE *logfile;
char buffer[256];
char resultfilename[64];
FILE *resultfile;
char commandfilename[64];
FILE *commandfile;

// Define serial properties
#define PORT "/dev/tty.usbserial-141120"
// #define PORT "dev/ttyUSB0"
#define BAUD_RATE B115200
#define DURATION 5 // Duration in seconds, set to 0 for indefinite logging
int serial_fd;

// Function to set up the serial port
int setup_serial(const char *port, int baud_rate);

// Function to log GNSS data indefinitely or for a specified duration
void *log_serial_data();

// Function to process GNSS data
extern volatile bool gga_ready_flag;
struct gga_data
{
    float UTC_time;
    double latitude;
    char lat_direction;
    double longitude;
    char lon_direction;
    int fix_quality;
    int num_satellites;
    float HDOP;
    double altitude;
    char alt_unit;
    float geoid_height;
    char geoid_unit;
};
void *process_nmea_data();

// Function to handle commands
void *handle_commands(void *command_id);

// Error codes and cleanups
void error_codes(const int code);
void cleanup();

#endif
