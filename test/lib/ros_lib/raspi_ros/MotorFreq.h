#ifndef _ROS_raspi_ros_MotorFreq_h
#define _ROS_raspi_ros_MotorFreq_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace raspi_ros
{

  class MotorFreq : public ros::Msg
  {
    public:
      typedef int16_t _wheelLeft_type;
      _wheelLeft_type wheelLeft;
      typedef int16_t _wheelRight_type;
      _wheelRight_type wheelRight;

    MotorFreq():
      wheelLeft(0),
      wheelRight(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_wheelLeft;
      u_wheelLeft.real = this->wheelLeft;
      *(outbuffer + offset + 0) = (u_wheelLeft.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_wheelLeft.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->wheelLeft);
      union {
        int16_t real;
        uint16_t base;
      } u_wheelRight;
      u_wheelRight.real = this->wheelRight;
      *(outbuffer + offset + 0) = (u_wheelRight.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_wheelRight.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->wheelRight);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_wheelLeft;
      u_wheelLeft.base = 0;
      u_wheelLeft.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_wheelLeft.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->wheelLeft = u_wheelLeft.real;
      offset += sizeof(this->wheelLeft);
      union {
        int16_t real;
        uint16_t base;
      } u_wheelRight;
      u_wheelRight.base = 0;
      u_wheelRight.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_wheelRight.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->wheelRight = u_wheelRight.real;
      offset += sizeof(this->wheelRight);
     return offset;
    }

    const char * getType(){ return "raspi_ros/MotorFreq"; };
    const char * getMD5(){ return "51103d632b8f9ee823eb68fd974ae686"; };

  };

}
#endif