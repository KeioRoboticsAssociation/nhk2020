// test code

#include "mbedserial.h"

Serial pc(USBTX, USBRX, 115200);

int main()
{
  Mbedserial m_serial(pc);

  while (1)
  {
    static float d[2] = {0.0, 10000.0};

    m_serial.float_write(d, 2);

    d[0] += 0.0001;
    d[1] -= 0.0001;

    wait(0.1);
  }
}