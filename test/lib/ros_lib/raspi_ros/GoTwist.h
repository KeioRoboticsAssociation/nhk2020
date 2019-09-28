#ifndef _ROS_raspi_ros_GoTwist_h
#define _ROS_raspi_ros_GoTwist_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace raspi_ros
{

  class GoTwist : public ros::Msg
  {
    public:
      typedef double _linear_x_type;
      _linear_x_type linear_x;
      typedef double _linear_y_type;
      _linear_y_type linear_y;
      typedef double _linear_z_type;
      _linear_z_type linear_z;
      typedef double _angular_x_type;
      _angular_x_type angular_x;
      typedef double _angular_y_type;
      _angular_y_type angular_y;
      typedef double _angular_z_type;
      _angular_z_type angular_z;

    GoTwist():
      linear_x(0),
      linear_y(0),
      linear_z(0),
      angular_x(0),
      angular_y(0),
      angular_z(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        double real;
        uint64_t base;
      } u_linear_x;
      u_linear_x.real = this->linear_x;
      *(outbuffer + offset + 0) = (u_linear_x.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_linear_x.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_linear_x.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_linear_x.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_linear_x.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_linear_x.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_linear_x.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_linear_x.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->linear_x);
      union {
        double real;
        uint64_t base;
      } u_linear_y;
      u_linear_y.real = this->linear_y;
      *(outbuffer + offset + 0) = (u_linear_y.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_linear_y.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_linear_y.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_linear_y.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_linear_y.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_linear_y.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_linear_y.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_linear_y.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->linear_y);
      union {
        double real;
        uint64_t base;
      } u_linear_z;
      u_linear_z.real = this->linear_z;
      *(outbuffer + offset + 0) = (u_linear_z.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_linear_z.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_linear_z.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_linear_z.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_linear_z.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_linear_z.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_linear_z.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_linear_z.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->linear_z);
      union {
        double real;
        uint64_t base;
      } u_angular_x;
      u_angular_x.real = this->angular_x;
      *(outbuffer + offset + 0) = (u_angular_x.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_angular_x.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_angular_x.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_angular_x.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_angular_x.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_angular_x.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_angular_x.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_angular_x.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->angular_x);
      union {
        double real;
        uint64_t base;
      } u_angular_y;
      u_angular_y.real = this->angular_y;
      *(outbuffer + offset + 0) = (u_angular_y.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_angular_y.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_angular_y.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_angular_y.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_angular_y.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_angular_y.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_angular_y.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_angular_y.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->angular_y);
      union {
        double real;
        uint64_t base;
      } u_angular_z;
      u_angular_z.real = this->angular_z;
      *(outbuffer + offset + 0) = (u_angular_z.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_angular_z.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_angular_z.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_angular_z.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_angular_z.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_angular_z.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_angular_z.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_angular_z.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->angular_z);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        double real;
        uint64_t base;
      } u_linear_x;
      u_linear_x.base = 0;
      u_linear_x.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_linear_x.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_linear_x.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_linear_x.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_linear_x.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_linear_x.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_linear_x.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_linear_x.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->linear_x = u_linear_x.real;
      offset += sizeof(this->linear_x);
      union {
        double real;
        uint64_t base;
      } u_linear_y;
      u_linear_y.base = 0;
      u_linear_y.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_linear_y.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_linear_y.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_linear_y.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_linear_y.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_linear_y.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_linear_y.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_linear_y.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->linear_y = u_linear_y.real;
      offset += sizeof(this->linear_y);
      union {
        double real;
        uint64_t base;
      } u_linear_z;
      u_linear_z.base = 0;
      u_linear_z.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_linear_z.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_linear_z.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_linear_z.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_linear_z.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_linear_z.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_linear_z.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_linear_z.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->linear_z = u_linear_z.real;
      offset += sizeof(this->linear_z);
      union {
        double real;
        uint64_t base;
      } u_angular_x;
      u_angular_x.base = 0;
      u_angular_x.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_angular_x.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_angular_x.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_angular_x.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_angular_x.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_angular_x.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_angular_x.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_angular_x.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->angular_x = u_angular_x.real;
      offset += sizeof(this->angular_x);
      union {
        double real;
        uint64_t base;
      } u_angular_y;
      u_angular_y.base = 0;
      u_angular_y.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_angular_y.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_angular_y.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_angular_y.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_angular_y.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_angular_y.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_angular_y.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_angular_y.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->angular_y = u_angular_y.real;
      offset += sizeof(this->angular_y);
      union {
        double real;
        uint64_t base;
      } u_angular_z;
      u_angular_z.base = 0;
      u_angular_z.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_angular_z.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_angular_z.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_angular_z.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_angular_z.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_angular_z.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_angular_z.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_angular_z.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->angular_z = u_angular_z.real;
      offset += sizeof(this->angular_z);
     return offset;
    }

    const char * getType(){ return "raspi_ros/GoTwist"; };
    const char * getMD5(){ return "3ac58878d00f42fbc731fa86d45253b5"; };

  };

}
#endif