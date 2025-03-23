#include "gnsstracker.h"

extern int serial_fd;
extern FILE *commandfile;
extern char commandfilename[64];

void command_reader(const char command[])
{
    char command_response[256];
    int command_read = read(serial_fd, command_response, sizeof(command_response) - 1);
    if (command_read > 0 && strncmp(command_response, "$PQTM", 5) == 0)
    {
        command_response[command_read] = '\0';
        fprintf(commandfile, "Response to %s: %s", command, command_response);
    }
}

void command_writer(const char command[])
{
    int command_write = write(serial_fd, command, strlen(command));
    if (command_write == -1 || command_write != (int)strlen(command))
    {
        error_codes(9);
    }
    usleep(50000); // 50 ms delay to allow response
    command_reader(command);
}

void *handle_commands(void *command_id)
{
    char command_char = (char)(intptr_t)command_id; // Properly cast the void * to char
    printf("Here is the command_id: %c\n", command_char);
    commandfile = fopen(commandfilename, "w");
    if (!commandfile)
    {
        error_codes(10);
        exit(10);
    }

    switch (command_char)
    {
    case 'V':
        command_writer("$PQTMVERNO*58\r\n");
        break;
    case 'S':
        command_writer("$PQTMSAVEPAR*5A\r\n");
        break;
    case 'G':
        command_writer("$PQTMGNSSSTART*51\r\n");
        break;
    case 'T':
        command_writer("$PQTMGNSSSTOP*09\r\n");
        break;
    case 'C':
        command_writer("$PQTMCOLD*1C\r\n");
        break;
    case 'W':
        command_writer("$PQTMWARM*4B\r\n");
        break;
    case 'H':
        command_writer("$PQTMHOT*11\r\n");
        break;
    case 'R':
        command_writer("$PQTMSRR*4B\r\n");
        break;
    default:
        break;
    }

    return NULL;
}
