#ifndef _ROS_raspimouse_ros_Switches_h
#define _ROS_raspimouse_ros_Switches_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace raspimouse_ros
{

  class Switches : public ros::Msg
  {
    public:
      typedef bool _front_type;
      _front_type front;
      typedef bool _center_type;
      _center_type center;
      typedef bool _rear_type;
      _rear_type rear;
      typedef const char* _state_type;
      _state_type state;

    Switches():
      front(0),
      center(0),
      rear(0),
      state("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_front;
      u_front.real = this->front;
      *(outbuffer + offset + 0) = (u_front.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->front);
      union {
        bool real;
        uint8_t base;
      } u_center;
      u_center.real = this->center;
      *(outbuffer + offset + 0) = (u_center.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->center);
      union {
        bool real;
        uint8_t base;
      } u_rear;
      u_rear.real = this->rear;
      *(outbuffer + offset + 0) = (u_rear.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->rear);
      uint32_t length_state = strlen(this->state);
      varToArr(outbuffer + offset, length_state);
      offset += 4;
      memcpy(outbuffer + offset, this->state, length_state);
      offset += length_state;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_front;
      u_front.base = 0;
      u_front.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->front = u_front.real;
      offset += sizeof(this->front);
      union {
        bool real;
        uint8_t base;
      } u_center;
      u_center.base = 0;
      u_center.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->center = u_center.real;
      offset += sizeof(this->center);
      union {
        bool real;
        uint8_t base;
      } u_rear;
      u_rear.base = 0;
      u_rear.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->rear = u_rear.real;
      offset += sizeof(this->rear);
      uint32_t length_state;
      arrToVar(length_state, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_state; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_state-1]=0;
      this->state = (char *)(inbuffer + offset-1);
      offset += length_state;
     return offset;
    }

    const char * getType(){ return "raspimouse_ros/Switches"; };
    const char * getMD5(){ return "72c4e0ea7f10bb6472900855b4640fa2"; };

  };

}
#endif