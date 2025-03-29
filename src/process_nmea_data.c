#include "gnsstracker.h"

struct gga_data gga;
extern char buffer[128];
extern char resultfilename[64];
extern FILE *resultfile;
extern int max_fixes;

char fix_quality_types[7][64] = {"Invalid Fix", "GPS SPS Fix", "DGNSS Fix", "GPS PPS Fix", "Real Time Kinematic (RTK) Fix", "Float RTK", "Estimated DR Fix"};
int epoch_counter = 1;
char newline = '\n';
char invalid_fix_message[64] = "Invalid Fix...";

void commandfile_logger()
{
    if (gga_ready_flag && (strstr(buffer, "$GNGGA")))
    {
        sscanf(buffer, "$GNGGA,%f,%lf,%c,%lf,%c,%d,%d,%f,%lf,%c,%f,%c",
               &gga.UTC_time, &gga.latitude, &gga.lat_direction, &gga.longitude, &gga.lon_direction,
               &gga.fix_quality, &gga.num_satellites, &gga.HDOP, &gga.altitude, &gga.alt_unit,
               &gga.geoid_height, &gga.geoid_unit);
        if (gga.fix_quality > 0)
        {
            fprintf(resultfile, "Fix count: %d\n", epoch_counter);
            fprintf(resultfile, "Fix type: %s\n", fix_quality_types[gga.fix_quality]);
            fprintf(resultfile, "UTC Time: %010.3f\n", gga.UTC_time);
            fprintf(resultfile, "Latitude: %.6f %c\n", gga.latitude, gga.lat_direction);
            fprintf(resultfile, "Longitude: %.6f %c\n", gga.longitude, gga.lon_direction);
            fprintf(resultfile, "Fix Quality: %d\n", gga.fix_quality);
            fprintf(resultfile, "Number of Satellites: %d\n", gga.num_satellites);
            fprintf(resultfile, "HDOP: %.3f\n", gga.HDOP);
            fprintf(resultfile, "Altitude: %.2f %c\n", gga.altitude, gga.alt_unit);
            fprintf(resultfile, "Geoid Height: %.2f %c\n", gga.geoid_height, gga.geoid_unit);
            fprintf(resultfile, "%c", newline);
            epoch_counter++;
            gga_ready_flag = false;
            usleep(50000); // 50 ms delay (fast enough to keep up with fastest message rate on LC29H)
        }
        else
        {
            fprintf(resultfile, "%s", invalid_fix_message);
            fprintf(resultfile, "%c", newline);
            usleep(50000); // 50 ms delay (fast enough to keep up with fastest message rate on LC29H)
        }
    }
    else
    {
        usleep(50000); // 50 ms delay (fast enough to keep up with fastest message rate on LC29H)
    }
}

void *process_nmea_data()
{

    resultfile = fopen(resultfilename, "w");
    if (!resultfile)
    {
        error_codes(8);
        exit(8);
    }

    if (DURATION == 0)
    {
        while (max_fixes > 0)
        {
            commandfile_logger();
            max_fixes--;
        }
    }

    else if (DURATION > 0)
    {
        time_t start_time = time(NULL);
        while (time(NULL) - start_time < DURATION)
        {
            commandfile_logger();
        }
    }

    else
    {
        error_codes(3);
        exit(3);
    }
    return NULL;
}
