#ifndef _ROS_SERVICE_SwitchIRCamera_h
#define _ROS_SERVICE_SwitchIRCamera_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace astra_camera
{

static const char SWITCHIRCAMERA[] = "astra_camera/SwitchIRCamera";

  class SwitchIRCameraRequest : public ros::Msg
  {
    public:
      typedef const char* _camera_type;
      _camera_type camera;

    SwitchIRCameraRequest():
      camera("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      uint32_t length_camera = strlen(this->camera);
      varToArr(outbuffer + offset, length_camera);
      offset += 4;
      memcpy(outbuffer + offset, this->camera, length_camera);
      offset += length_camera;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      uint32_t length_camera;
      arrToVar(length_camera, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_camera; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_camera-1]=0;
      this->camera = (char *)(inbuffer + offset-1);
      offset += length_camera;
     return offset;
    }

    virtual const char * getType() override { return SWITCHIRCAMERA; };
    virtual const char * getMD5() override { return "8162e9a89f39d80b11a715b34e890034"; };

  };

  class SwitchIRCameraResponse : public ros::Msg
  {
    public:

    SwitchIRCameraResponse()
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

    virtual const char * getType() override { return SWITCHIRCAMERA; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class SwitchIRCamera {
    public:
    typedef SwitchIRCameraRequest Request;
    typedef SwitchIRCameraResponse Response;
  };

}
#endif
