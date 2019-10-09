// mbedserial.h
// serial communication : mbed <-> ROS

#ifndef _MBEDSERIAL_
#define _MBEDSERIAL_

#include <mbed.h>
#include <stdio.h>
#include <string.h>

// mbedserial class
class Mbedserial
{
private:
	char *msg_buf;
	int bufsize;
	char endmsg;
	Serial& rospc;
	void rcv_callback();

public:
	float getfloat[32];
	int getint[32];
	char getchar[256];
	int floatarraysize;
	int intarraysize;
	int chararraysize;
	Mbedserial(Serial&);
	void float_write(float *array, int arraysize);
	void int_write(int *array, int arraysize);
	void char_write(char *array, int arraysize);
};

#endif