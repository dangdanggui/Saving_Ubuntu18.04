#ifndef _ROS_SERVICE_ResetIRExposure_h
#define _ROS_SERVICE_ResetIRExposure_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace astra_camera
{

static const char RESETIREXPOSURE[] = "astra_camera/ResetIRExposure";

  class ResetIRExposureRequest : public ros::Msg
  {
    public:

    ResetIRExposureRequest()
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

    virtual const char * getType() override { return RESETIREXPOSURE; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class ResetIRExposureResponse : public ros::Msg
  {
    public:

    ResetIRExposureResponse()
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

    virtual const char * getType() override { return RESETIREXPOSURE; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class ResetIRExposure {
    public:
    typedef ResetIRExposureRequest Request;
    typedef ResetIRExposureResponse Response;
  };

}
#endif
