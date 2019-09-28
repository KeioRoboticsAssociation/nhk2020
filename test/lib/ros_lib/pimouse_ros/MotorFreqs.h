#ifndef _ROS_pimouse_ros_MotorFreqs_h
#define _ROS_pimouse_ros_MotorFreqs_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace pimouse_ros
{

  class MotorFreqs : public ros::Msg
  {
    public:
      typedef int16_t _left_hz_type;
      _left_hz_type left_hz;
      typedef int16_t _right_hz_type;
      _right_hz_type right_hz;

    MotorFreqs():
      left_hz(0),
      right_hz(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_left_hz;
      u_left_hz.real = this->left_hz;
      *(outbuffer + offset + 0) = (u_left_hz.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_left_hz.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->left_hz);
      union {
        int16_t real;
        uint16_t base;
      } u_right_hz;
      u_right_hz.real = this->right_hz;
      *(outbuffer + offset + 0) = (u_right_hz.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_right_hz.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->right_hz);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_left_hz;
      u_left_hz.base = 0;
      u_left_hz.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_left_hz.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->left_hz = u_left_hz.real;
      offset += sizeof(this->left_hz);
      union {
        int16_t real;
        uint16_t base;
      } u_right_hz;
      u_right_hz.base = 0;
      u_right_hz.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_right_hz.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->right_hz = u_right_hz.real;
      offset += sizeof(this->right_hz);
     return offset;
    }

    const char * getType(){ return "pimouse_ros/MotorFreqs"; };
    const char * getMD5(){ return "921a8bc2b9eda90f5d3ca620a3549e13"; };

  };

}
#endif