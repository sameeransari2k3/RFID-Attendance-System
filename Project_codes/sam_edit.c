/*main.c: Very simple program to test the serial port. Expects the port to be looped back to itself */


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "uart.h"

int main ()
{
	int fd ;
	int tx,rx ;

	puts("Opening serial port\n");

	if ((fd = serialOpen ("/dev/ttyUSB0",9600)) < 0)
	{

		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 1 ;
	}

	puts("serial port is opened\n");

	usleep(1000000);

	 if ((fd = serialOpen("/dev/ttyUSB0", 9600)) < 0) {
        fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
        return 1;
    }

    puts("Serial port opened. Waiting for data (RFID,Date)...");

    char buffer[100];
    int index = 0;
    while (1) 
    {
        char ch = serialGetchar(fd);

        if (ch == '$')
	{
            buffer[index] = '\0'; 
            printf("Received: %s\n", buffer);

            char rfid[30], date[20];
            sscanf(buffer, "%[^,],%s", rfid, date);

            printf("RFID ID : %s\n", rfid);
            printf("Date    : %s\n", date);

            index = 0; 
        } 
	else 
	{
            if (index < sizeof(buffer) - 1)
                buffer[index++] = ch;
        }
    }
	serialClose(fd);
	puts("Closing serial port\n");
	return 0 ;
}

