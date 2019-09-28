#ifndef _ROS_raspimouse_ros_LightSensorValues_h
#define _ROS_raspimouse_ros_LightSensorValues_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace raspimouse_ros
{

  class LightSensorValues : public ros::Msg
  {
    public:
      typedef int16_t _right_forward_type;
      _right_forward_type right_forward;
      typedef int16_t _right_side_type;
      _right_side_type right_side;
      typedef int16_t _left_side_type;
      _left_side_type left_side;
      typedef int16_t _left_forward_type;
      _left_forward_type left_forward;

    LightSensorValues():
      right_forward(0),
      right_side(0),
      left_side(0),
      left_forward(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_right_forward;
      u_right_forward.real = this->right_forward;
      *(outbuffer + offset + 0) = (u_right_forward.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_right_forward.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->right_forward);
      union {
        int16_t real;
        uint16_t base;
      } u_right_side;
      u_right_side.real = this->right_side;
      *(outbuffer + offset + 0) = (u_right_side.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_right_side.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->right_side);
      union {
        int16_t real;
        uint16_t base;
      } u_left_side;
      u_left_side.real = this->left_side;
      *(outbuffer + offset + 0) = (u_left_side.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_left_side.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->left_side);
      union {
        int16_t real;
        uint16_t base;
      } u_left_forward;
      u_left_forward.real = this->left_forward;
      *(outbuffer + offset + 0) = (u_left_forward.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_left_forward.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->left_forward);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_right_forward;
      u_right_forward.base = 0;
      u_right_forward.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_right_forward.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->right_forward = u_right_forward.real;
      offset += sizeof(this->right_forward);
      union {
        int16_t real;
        uint16_t base;
      } u_right_side;
      u_right_side.base = 0;
      u_right_side.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_right_side.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->right_side = u_right_side.real;
      offset += sizeof(this->right_side);
      union {
        int16_t real;
        uint16_t base;
      } u_left_side;
      u_left_side.base = 0;
      u_left_side.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_left_side.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->left_side = u_left_side.real;
      offset += sizeof(this->left_side);
      union {
        int16_t real;
        uint16_t base;
      } u_left_forward;
      u_left_forward.base = 0;
      u_left_forward.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_left_forward.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->left_forward = u_left_forward.real;
      offset += sizeof(this->left_forward);
     return offset;
    }

    const char * getType(){ return "raspimouse_ros/LightSensorValues"; };
    const char * getMD5(){ return "fa26acaa6485ef64ceca927a62524c60"; };

  };

}
#endif