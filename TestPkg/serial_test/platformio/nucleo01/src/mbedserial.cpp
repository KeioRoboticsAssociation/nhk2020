// mbedserial.cpp
// serial communication : mbed <-> ROS

#include "mbedserial.h"

/***********************init***********************/

Mbedserial::Mbedserial(Serial& pc) : rospc(pc)
{
    msg_buf = new char[256];
    bufsize = 0;
	endmsg = '\n';
	floatarraysize = 0;
	intarraysize = 0;
	chararraysize = 0;

    //rospc = pc;
    rospc.attach(callback(this,&Mbedserial::rcv_callback), Serial::RxIrq);
}

/**********************receive**********************/

void Mbedserial::rcv_callback()
{
    msg_buf[bufsize] = rospc.getc();
    if (msg_buf[bufsize] == endmsg)
    {
        switch (msg_buf[0])
        {
        case 'f':
            memcpy(&floatarraysize, &msg_buf[1], 4);
            if (bufsize == floatarraysize * 4 + 6)
            {
                for (int i = 0; i < floatarraysize; i++)
                {
                    memcpy(&getfloat[i], &msg_buf[i * 4 + 5], 4);
                }
            }
            break;
        case 'i':
            memcpy(&intarraysize, &msg_buf[1], 4);
            if (bufsize == intarraysize * 4 + 6)
            {
                for (int i = 0; i < intarraysize; i++)
                {
                    memcpy(&getint[i], &msg_buf[i * 4 + 5], 4);
                }
            }
            break;
        case 'c':
            memcpy(&chararraysize, &msg_buf[1], 4);
            if (bufsize == chararraysize + 6)
            {
                memcpy(&getchar[0], &msg_buf[5], chararraysize);
            }
            break;
        }
        delete[] msg_buf;
        bufsize = 0;
        msg_buf = new char[256];
    }
    else
    {
        bufsize++;
    }
}

/***********************send***********************/

void Mbedserial::float_write(float *array, int arraysize)
{
    // send data type
    char msg_type = 'f';
    rospc.putc(msg_type);

    // send array size
    char arraysize_c[4];
    memcpy(arraysize_c, &arraysize, 4);
    for (int i = 0; i < 4; i++)
    {
        rospc.putc(arraysize_c[i]);
    }

    // send float data
    char array_c[4];
    for (int i = 0; i < arraysize; i++)
    {
        memcpy(array_c, &array[i], 4);
        for (int j = 0; j < 4; j++)
        {
            rospc.putc(array_c[j]);
        }
    }

    // send end message
    rospc.putc(endmsg);

    /*
	char itoc[4];
	char ftoc[4];
	char *serialchar;
	serialchar = new char[4 * (finsize + 1)];
	memcpy(&itoc, &finsize, 4);
	//strcat_s(serialchar, sizeof(serialchar), itoc);

	for (int i = 0; i < 4; i++) {
		serialchar[i] = itoc[i];
	}

	for (int i = 0; i < finsize; i++) {
		memcpy(&ftoc, &fin[i], 4);
		
		for (int j = 0; j < 4; j++) {
			serialchar[4*(i+1)+j] = ftoc[j];
		}
		//strcat(serialchar, ftoc);
	}

	cout << "send : ";
	for (int i = 0; i < 4 * (finsize + 1); i++) {
	}

    int ansint = 20;
    float *ansflt;
    memcpy(&ansint, serialchar, 4);
    ansflt = new float[ansint];
    for (int i = 0; i < ansint; i++)
    {
        memcpy(&ansflt[i], &serialchar[4 * (i + 1)], 4);
    }

    cout << "ansint : " << ansint << endl;
    cout << "ansflt";
    for (int i = 0; i < ansint; i++)
    {
        cout << " : " << ansflt[i];
    }
    cout << endl;

    delete[] serialchar;

    delete[] ansflt;
    */
}

void Mbedserial::int_write(int *array, int arraysize)
{
    // send data type
    char msg_type = 'i';
    rospc.putc(msg_type);

    // send array size
    char arraysize_c[4];
    memcpy(arraysize_c, &arraysize, 4);
    for (int i = 0; i < 4; i++)
    {
        rospc.putc(arraysize_c[i]);
    }

    // send int data
    char array_c[4];
    for (int i = 0; i < arraysize; i++)
    {
        memcpy(array_c, &array[i], 4);
        for (int j = 0; j < 4; j++)
        {
            rospc.putc(array_c[j]);
        }
    }

    // send end message
    rospc.putc(endmsg);
}

void Mbedserial::char_write(char *array, int arraysize)
{
    // send data type
    char msg_type = 'c';
    rospc.putc(msg_type);

    // send array size
    char arraysize_c[4];
    memcpy(arraysize_c, &arraysize, 4);
    for (int i = 0; i < 4; i++)
    {
        rospc.putc(arraysize_c[i]);
    }

    // send char data
    for (int i = 0; i < arraysize; i++)
    {
        rospc.putc(array[i]);
    }

    // send end message
    rospc.putc(endmsg);
}