/**
 * @file gnsstracker.c
 * @brief GNSS Tracker Application
 *
 * This program initializes and runs a GNSS tracker application. It sets up a serial
 * connection to a GNSS device, logs GNSS data, processes NMEA sentences, and handles
 * user commands via threads.
 *
 * The program performs the following tasks:
 * - Initializes the serial port with a specified port and baud rate.
 * - Sets up logging resources for GNSS data.
 * - Creates and manages threads for command handling, GNSS data logging, and NMEA data processing.
 * - Ensures proper cleanup of resources upon program exit.
 *
 * Threads:
 * - Command Thread: Handles user commands.
 * - Log Thread: Logs GNSS data from the serial port.
 * - NMEA Thread: Processes NMEA sentences from the GNSS data.
 *
 * Error Handling:
 * - The program uses `error_codes()` to handle various error scenarios, such as
 *   invalid serial port configuration, thread creation failures, and thread joining failures.
 *
 * Cleanup:
 * - The `atexit()` function is used to register a cleanup function that ensures
 *   proper resource deallocation, such as closing the serial port.
 *
 * @note The program exits with specific error codes in case of failures.
 * @note The `DURATION` macro determines how long the threads will run.
 *
 * @author Ade
 * @date 2025-09-01
 */
#include "src/gnsstracker.h"

int main()
{
    // Register the cleanup function to be called at program exit
    // The cleanup function will close the serial port and perform any necessary cleanup
    atexit(cleanup);

    // Welcome message
    printf("========== Ade' GNSS Tracker ==========\n");
    printf("Initializing...\n");

    // The setup_serial() function sets up the serial port with the specified port and baud rate.
    if (PORT == NULL || BAUD_RATE <= 0)
    {
        error_codes(1);
        exit(1);
    }
    serial_fd = setup_serial(PORT, BAUD_RATE);
    if (serial_fd == -1)
    {
        error_codes(1);
        exit(1);
    }

    // The init() function initializes the log file and sets up any necessary resources for logging GNSS data.
    init();

    // Create thread for command handling
    pthread_t command_thread;
    int command_thread_status = pthread_create(&command_thread, NULL, handle_commands, (void *)(intptr_t)'G');
    printf("Command thread status: %i\n", command_thread_status);
    if (command_thread_status != 0)
    {
        error_codes(11);
        exit(11);
    }

    // Create thread to log GNSS data
    pthread_t log_thread;
    int log_thread_status = pthread_create(&log_thread, NULL, log_serial_data, NULL);
    printf("Log thread status: %i\n", log_thread_status);
    if (log_thread_status != 0)
    {
        error_codes(4);
        exit(4);
    }

    // Create thread for processing GNSS data
    pthread_t nmea_thread;
    int nmea_thread_status = pthread_create(&nmea_thread, NULL, process_nmea_data, NULL);
    printf("NMEA thread status: %i\n", nmea_thread_status);
    if (nmea_thread_status != 0)
    {
        error_codes(6);
        exit(6);
    }

    // Wait for command thread to finish. Wait for 1 day if DURATION is set to 0.
    if (command_thread_status == 0)
    {
        if (pthread_join(command_thread, NULL) != 0)
        {
            error_codes(11);
            exit(11);
        }
        printf("Command thread status: %i\n", command_thread_status);
    }

    // Wait for log thread to finish. Wait for 1 day if DURATION is set to 0.
    if (log_thread_status == 0)
    {
        if (pthread_join(log_thread, NULL) != 0)
        {
            error_codes(5);
            exit(5);
        }
        printf("Log thread status: %i\n", log_thread_status);
    }

    // Wait for nmea processing thread to finish. Wait for 1 day if DURATION is set to 0.
    if (nmea_thread_status == 0)
    {
        if (pthread_join(nmea_thread, NULL) != 0)
        {
            error_codes(7);
            exit(7);
        }
        printf("NMEA thread status: %i\n", nmea_thread_status);
    }

    return 0;
}
