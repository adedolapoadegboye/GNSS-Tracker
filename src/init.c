#include "gnsstracker.h"

extern char filename[64];
extern char resultfilename[64];
extern char commandfilename[64];

void init()
{
    // Ensure log folder exists for storing logs
    system("mkdir -p logs");

    // Set up logfile name
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(filename, sizeof(filename), "logs/gnss_logs_%Y%m%d_%H%M%S.txt", tm_info);
    strftime(resultfilename, sizeof(resultfilename), "logs/gnss_results_%Y%m%d_%H%M%S.txt", tm_info);
    strftime(commandfilename, sizeof(commandfilename), "logs/gnss_commands_%Y%m%d_%H%M%S.txt", tm_info);
    // handle_commands((void *)(intptr_t)'V');
    handle_commands((void *)(intptr_t)'G');
}
