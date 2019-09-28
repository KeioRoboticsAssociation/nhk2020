#ifndef _ROS_SERVICE_SwitchMotors_h
#define _ROS_SERVICE_SwitchMotors_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace raspimouse_ros
{

static const char SWITCHMOTORS[] = "raspimouse_ros/SwitchMotors";

  class SwitchMotorsRequest : public ros::Msg
  {
    public:
      typedef bool _on_type;
      _on_type on;

    SwitchMotorsRequest():
      on(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_on;
      u_on.real = this->on;
      *(outbuffer + offset + 0) = (u_on.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->on);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_on;
      u_on.base = 0;
      u_on.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->on = u_on.real;
      offset += sizeof(this->on);
     return offset;
    }

    const char * getType(){ return SWITCHMOTORS; };
    const char * getMD5(){ return "74983d2ffe4877de8ae30b7a94625c41"; };

  };

  class SwitchMotorsResponse : public ros::Msg
  {
    public:
      typedef bool _accepted_type;
      _accepted_type accepted;

    SwitchMotorsResponse():
      accepted(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_accepted;
      u_accepted.real = this->accepted;
      *(outbuffer + offset + 0) = (u_accepted.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->accepted);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_accepted;
      u_accepted.base = 0;
      u_accepted.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->accepted = u_accepted.real;
      offset += sizeof(this->accepted);
     return offset;
    }

    const char * getType(){ return SWITCHMOTORS; };
    const char * getMD5(){ return "1ea39b897cc8620c46aaa14cb60920cd"; };

  };

  class SwitchMotors {
    public:
    typedef SwitchMotorsRequest Request;
    typedef SwitchMotorsResponse Response;
  };

}
#endif
