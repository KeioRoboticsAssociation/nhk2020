#ifndef _ROS_raspi_ros_LightSensors_h
#define _ROS_raspi_ros_LightSensors_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace raspi_ros
{

  class LightSensors : public ros::Msg
  {
    public:
      typedef int16_t _r_side_type;
      _r_side_type r_side;
      typedef int16_t _r_front_type;
      _r_front_type r_front;
      typedef int16_t _l_front_type;
      _l_front_type l_front;
      typedef int16_t _l_side_type;
      _l_side_type l_side;
      typedef int16_t _sum_type;
      _sum_type sum;
      typedef int16_t _sum_forward_type;
      _sum_forward_type sum_forward;

    LightSensors():
      r_side(0),
      r_front(0),
      l_front(0),
      l_side(0),
      sum(0),
      sum_forward(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_r_side;
      u_r_side.real = this->r_side;
      *(outbuffer + offset + 0) = (u_r_side.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_r_side.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->r_side);
      union {
        int16_t real;
        uint16_t base;
      } u_r_front;
      u_r_front.real = this->r_front;
      *(outbuffer + offset + 0) = (u_r_front.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_r_front.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->r_front);
      union {
        int16_t real;
        uint16_t base;
      } u_l_front;
      u_l_front.real = this->l_front;
      *(outbuffer + offset + 0) = (u_l_front.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_l_front.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->l_front);
      union {
        int16_t real;
        uint16_t base;
      } u_l_side;
      u_l_side.real = this->l_side;
      *(outbuffer + offset + 0) = (u_l_side.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_l_side.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->l_side);
      union {
        int16_t real;
        uint16_t base;
      } u_sum;
      u_sum.real = this->sum;
      *(outbuffer + offset + 0) = (u_sum.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_sum.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->sum);
      union {
        int16_t real;
        uint16_t base;
      } u_sum_forward;
      u_sum_forward.real = this->sum_forward;
      *(outbuffer + offset + 0) = (u_sum_forward.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_sum_forward.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->sum_forward);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_r_side;
      u_r_side.base = 0;
      u_r_side.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_r_side.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->r_side = u_r_side.real;
      offset += sizeof(this->r_side);
      union {
        int16_t real;
        uint16_t base;
      } u_r_front;
      u_r_front.base = 0;
      u_r_front.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_r_front.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->r_front = u_r_front.real;
      offset += sizeof(this->r_front);
      union {
        int16_t real;
        uint16_t base;
      } u_l_front;
      u_l_front.base = 0;
      u_l_front.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_l_front.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->l_front = u_l_front.real;
      offset += sizeof(this->l_front);
      union {
        int16_t real;
        uint16_t base;
      } u_l_side;
      u_l_side.base = 0;
      u_l_side.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_l_side.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->l_side = u_l_side.real;
      offset += sizeof(this->l_side);
      union {
        int16_t real;
        uint16_t base;
      } u_sum;
      u_sum.base = 0;
      u_sum.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_sum.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->sum = u_sum.real;
      offset += sizeof(this->sum);
      union {
        int16_t real;
        uint16_t base;
      } u_sum_forward;
      u_sum_forward.base = 0;
      u_sum_forward.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_sum_forward.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->sum_forward = u_sum_forward.real;
      offset += sizeof(this->sum_forward);
     return offset;
    }

    const char * getType(){ return "raspi_ros/LightSensors"; };
    const char * getMD5(){ return "dfd92ecbcddc4804aebf31a47e69a45b"; };

  };

}
#endif