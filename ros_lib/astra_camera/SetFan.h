#ifndef _ROS_SERVICE_SetFan_h
#define _ROS_SERVICE_SetFan_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace astra_camera
{

static const char SETFAN[] = "astra_camera/SetFan";

  class SetFanRequest : public ros::Msg
  {
    public:
      typedef bool _enable_type;
      _enable_type enable;

    SetFanRequest():
      enable(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_enable;
      u_enable.real = this->enable;
      *(outbuffer + offset + 0) = (u_enable.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->enable);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_enable;
      u_enable.base = 0;
      u_enable.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->enable = u_enable.real;
      offset += sizeof(this->enable);
     return offset;
    }

    virtual const char * getType() override { return SETFAN; };
    virtual const char * getMD5() override { return "8c1211af706069c994c06e00eb59ac9e"; };

  };

  class SetFanResponse : public ros::Msg
  {
    public:

    SetFanResponse()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
     return offset;
    }

    virtual const char * getType() override { return SETFAN; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class SetFan {
    public:
    typedef SetFanRequest Request;
    typedef SetFanResponse Response;
  };

}
#endif
