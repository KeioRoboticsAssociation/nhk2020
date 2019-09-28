#ifndef _ROS_SERVICE_PutMotorFreqs_h
#define _ROS_SERVICE_PutMotorFreqs_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace raspimouse_ros
{

static const char PUTMOTORFREQS[] = "raspimouse_ros/PutMotorFreqs";

  class PutMotorFreqsRequest : public ros::Msg
  {
    public:
      typedef int16_t _left_type;
      _left_type left;
      typedef int16_t _right_type;
      _right_type right;
      typedef int32_t _duration_type;
      _duration_type duration;

    PutMotorFreqsRequest():
      left(0),
      right(0),
      duration(0)
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
      union {
        int32_t real;
        uint32_t base;
      } u_duration;
      u_duration.real = this->duration;
      *(outbuffer + offset + 0) = (u_duration.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_duration.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_duration.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_duration.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->duration);
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
      union {
        int32_t real;
        uint32_t base;
      } u_duration;
      u_duration.base = 0;
      u_duration.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_duration.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_duration.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_duration.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->duration = u_duration.real;
      offset += sizeof(this->duration);
     return offset;
    }

    const char * getType(){ return PUTMOTORFREQS; };
    const char * getMD5(){ return "c844e9c321acd0da2750307eb75f9e34"; };

  };

  class PutMotorFreqsResponse : public ros::Msg
  {
    public:
      typedef bool _accepted_type;
      _accepted_type accepted;

    PutMotorFreqsResponse():
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

    const char * getType(){ return PUTMOTORFREQS; };
    const char * getMD5(){ return "1ea39b897cc8620c46aaa14cb60920cd"; };

  };

  class PutMotorFreqs {
    public:
    typedef PutMotorFreqsRequest Request;
    typedef PutMotorFreqsResponse Response;
  };

}
#endif
