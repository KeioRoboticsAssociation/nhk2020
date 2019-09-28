#ifndef _ROS_SERVICE_TimedMotion_h
#define _ROS_SERVICE_TimedMotion_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace pimouse_ros
{

static const char TIMEDMOTION[] = "pimouse_ros/TimedMotion";

  class TimedMotionRequest : public ros::Msg
  {
    public:
      typedef int16_t _left_hz_type;
      _left_hz_type left_hz;
      typedef int16_t _right_hz_type;
      _right_hz_type right_hz;
      typedef uint32_t _duration_ms_type;
      _duration_ms_type duration_ms;

    TimedMotionRequest():
      left_hz(0),
      right_hz(0),
      duration_ms(0)
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
      *(outbuffer + offset + 0) = (this->duration_ms >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->duration_ms >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->duration_ms >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->duration_ms >> (8 * 3)) & 0xFF;
      offset += sizeof(this->duration_ms);
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
      this->duration_ms =  ((uint32_t) (*(inbuffer + offset)));
      this->duration_ms |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->duration_ms |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->duration_ms |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->duration_ms);
     return offset;
    }

    const char * getType(){ return TIMEDMOTION; };
    const char * getMD5(){ return "d70b1c52f947559f961bed5aa1647139"; };

  };

  class TimedMotionResponse : public ros::Msg
  {
    public:
      typedef bool _success_type;
      _success_type success;

    TimedMotionResponse():
      success(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_success;
      u_success.real = this->success;
      *(outbuffer + offset + 0) = (u_success.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->success);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_success;
      u_success.base = 0;
      u_success.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->success = u_success.real;
      offset += sizeof(this->success);
     return offset;
    }

    const char * getType(){ return TIMEDMOTION; };
    const char * getMD5(){ return "358e233cde0c8a8bcfea4ce193f8fc15"; };

  };

  class TimedMotion {
    public:
    typedef TimedMotionRequest Request;
    typedef TimedMotionResponse Response;
  };

}
#endif
