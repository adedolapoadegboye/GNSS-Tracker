#include "gnsstracker.h"

extern char filename[64];
extern int serial_fd;
extern FILE *logfile;
extern FILE *resultfile;
extern FILE *commandfile;

void error_codes(const int code)
{
    switch (code)
    {
    case 1:
        printf("Error: Unable to open serial port\n");
        break;
    case 2:
        printf("Error: Unable to open log file\n");
        break;
    case 3:
        printf("Error: Invalid runtime duration\n");
        break;
    case 4:
        printf("Error: Unable to create log thread\n");
        break;
    case 5:
        printf("Error: Unable to join log thread\n");
        break;
    case 6:
        printf("Error: Unable to create NMEA thread\n");
        break;
    case 7:
        printf("Error: Unable to join NMEA thread\n");
        break;
    case 8:
        printf("Error: Unable to open result file\n");
        break;
    case 9:
        printf("Error: Unable to write command to serial port\n");
        break;
    case 10:
        printf("Error: Unable to open command file\n");
        break;
    case 11:
        printf("Error: Unable to create command thread\n");
        break;
    default:
        printf("Error: Unknown error\n");
        break;
    }
}

void cleanup()
{
    printf("Cleaning up...\n");
    handle_commands((void *)(intptr_t)'T');
    close(serial_fd);
    fclose(logfile);
    fclose(resultfile);
    fclose(commandfile);
    printf("Exiting...\n");
}
