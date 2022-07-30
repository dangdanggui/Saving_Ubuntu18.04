#ifndef _ROS_SERVICE_SetDistortioncal_h
#define _ROS_SERVICE_SetDistortioncal_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace astra_camera
{

static const char SETDISTORTIONCAL[] = "astra_camera/SetDistortioncal";

  class SetDistortioncalRequest : public ros::Msg
  {
    public:
      typedef bool _enable_type;
      _enable_type enable;

    SetDistortioncalRequest():
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

    virtual const char * getType() override { return SETDISTORTIONCAL; };
    virtual const char * getMD5() override { return "8c1211af706069c994c06e00eb59ac9e"; };

  };

  class SetDistortioncalResponse : public ros::Msg
  {
    public:

    SetDistortioncalResponse()
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

    virtual const char * getType() override { return SETDISTORTIONCAL; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class SetDistortioncal {
    public:
    typedef SetDistortioncalRequest Request;
    typedef SetDistortioncalResponse Response;
  };

}
#endif
