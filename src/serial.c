#include "gnsstracker.h"

extern char filename[64];
extern int serial_fd;
extern FILE *logfile;
volatile bool gga_ready_flag = false;
extern char buffer[128];
int max_fixes = 86400;

// Function to set up the serial port
int setup_serial(const char *port, int baud_rate)
{
    int fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)
    {
        perror("Error opening serial port");
        return -1;
    }

    struct termios options;
    tcgetattr(fd, &options);

    cfsetispeed(&options, baud_rate);
    cfsetospeed(&options, baud_rate);

    options.c_cflag = baud_rate | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &options);

    return fd;
}

// Function to log GNSS data
void *log_serial_data()
{
    time_t start_time = time(NULL);

    logfile = fopen(filename, "w");
    if (!logfile)
    {
        error_codes(2);
        exit(2);
    }
    if (DURATION > 0)
    {
        printf("Logging GNSS data to %s for %d seconds...\n", filename, DURATION);
        handle_commands((void *)(intptr_t)'G');
        while (time(NULL) - start_time < DURATION)
        {
            int bytes_read = read(serial_fd, buffer, sizeof(buffer) - 1);
            if (bytes_read > 0)
            {
                buffer[bytes_read] = '\0';
                fprintf(logfile, "%s", buffer);
                printf("%s", buffer); // Print to console
                if (strncmp(buffer, "$GNGGA", 6) == 0)
                {
                    gga_ready_flag = true;
                }
            }
            usleep(50000); // Small delay (equivalent to fastest message rate on LC29H)
        }
    }
    else if (DURATION == 0)
    {
        printf("Logging GNSS data to %s...\n", filename);
        handle_commands((void *)(intptr_t)'G');
        while (max_fixes > 0)
        {
            int bytes_read = read(serial_fd, buffer, sizeof(buffer) - 1);
            if (bytes_read > 0)
            {
                buffer[bytes_read] = '\0';
                fprintf(logfile, "%s", buffer);
                printf("%s", buffer); // Print to console
                if (strncmp(buffer, "$GNGGA", 6) == 0)
                {
                    gga_ready_flag = true;
                }
            }
            usleep(50000); // Small delay (equivalent to fastest message rate on LC29H)
            max_fixes--;
        }
    }
    else
    {
        printf("Invalid duration specified\n");
        error_codes(3);
        exit(3);
    }

    fclose(logfile);
    printf("\nLogging complete. Data saved to %s\n", filename);
    return NULL;
}
