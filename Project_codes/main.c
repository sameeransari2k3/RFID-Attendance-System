/*main.c: Very simple program to test the serial port. Expects the port to be looped back to itself */


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
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

	for (tx = 'A' ; tx <'A'+26 ;tx++ )
	{
		serialPutchar(fd, tx) ;
		printf("sent  : %c\n",tx);
		rx=serialGetchar(fd);
		printf ("recvd : %c\n",rx) ;
		usleep(100000);
		system("clear");
	}

	printf ("\n") ;
	serialClose(fd);
	puts("Closing serial port\n");
	return 0 ;
}

