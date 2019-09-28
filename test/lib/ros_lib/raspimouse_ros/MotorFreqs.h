#ifndef _ROS_raspimouse_ros_MotorFreqs_h
#define _ROS_raspimouse_ros_MotorFreqs_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace raspimouse_ros
{

  class MotorFreqs : public ros::Msg
  {
    public:
      typedef int16_t _left_type;
      _left_type left;
      typedef int16_t _right_type;
      _right_type right;

    MotorFreqs():
      left(0),
      right(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_left;
      u_left.real = this->left;
      *(outbuffer + offset + 0) = (u_left.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_left.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->left);
      union {
        int16_t real;
        uint16_t base;
      } u_right;
      u_right.real = this->right;
      *(outbuffer + offset + 0) = (u_right.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_right.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->right);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_left;
      u_left.base = 0;
      u_left.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_left.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->left = u_left.real;
      offset += sizeof(this->left);
      union {
        int16_t real;
        uint16_t base;
      } u_right;
      u_right.base = 0;
      u_right.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_right.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->right = u_right.real;
      offset += sizeof(this->right);
     return offset;
    }

    const char * getType(){ return "raspimouse_ros/MotorFreqs"; };
    const char * getMD5(){ return "09d1b2323a1aeae9343e81809a820b60"; };

  };

}
#endif