#ifndef _ROS_SERVICE_GetDeviceType_h
#define _ROS_SERVICE_GetDeviceType_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace astra_camera
{

static const char GETDEVICETYPE[] = "astra_camera/GetDeviceType";

  class GetDeviceTypeRequest : public ros::Msg
  {
    public:

    GetDeviceTypeRequest()
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

    virtual const char * getType() override { return GETDEVICETYPE; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class GetDeviceTypeResponse : public ros::Msg
  {
    public:
      typedef const char* _device_type_type;
      _device_type_type device_type;

    GetDeviceTypeResponse():
      device_type("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      uint32_t length_device_type = strlen(this->device_type);
      varToArr(outbuffer + offset, length_device_type);
      offset += 4;
      memcpy(outbuffer + offset, this->device_type, length_device_type);
      offset += length_device_type;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      uint32_t length_device_type;
      arrToVar(length_device_type, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_device_type; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_device_type-1]=0;
      this->device_type = (char *)(inbuffer + offset-1);
      offset += length_device_type;
     return offset;
    }

    virtual const char * getType() override { return GETDEVICETYPE; };
    virtual const char * getMD5() override { return "4c8e9dd50b39344412b92ce9e1e9615c"; };

  };

  class GetDeviceType {
    public:
    typedef GetDeviceTypeRequest Request;
    typedef GetDeviceTypeResponse Response;
  };

}
#endif
