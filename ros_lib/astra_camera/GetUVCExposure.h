#ifndef _ROS_SERVICE_GetUVCExposure_h
#define _ROS_SERVICE_GetUVCExposure_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace astra_camera
{

static const char GETUVCEXPOSURE[] = "astra_camera/GetUVCExposure";

  class GetUVCExposureRequest : public ros::Msg
  {
    public:

    GetUVCExposureRequest()
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

    virtual const char * getType() override { return GETUVCEXPOSURE; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class GetUVCExposureResponse : public ros::Msg
  {
    public:
      typedef int32_t _exposure_type;
      _exposure_type exposure;

    GetUVCExposureResponse():
      exposure(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        int32_t real;
        uint32_t base;
      } u_exposure;
      u_exposure.real = this->exposure;
      *(outbuffer + offset + 0) = (u_exposure.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_exposure.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_exposure.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_exposure.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->exposure);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        int32_t real;
        uint32_t base;
      } u_exposure;
      u_exposure.base = 0;
      u_exposure.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_exposure.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_exposure.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_exposure.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->exposure = u_exposure.real;
      offset += sizeof(this->exposure);
     return offset;
    }

    virtual const char * getType() override { return GETUVCEXPOSURE; };
    virtual const char * getMD5() override { return "0535ed1a1204bb999d0e7bea8f6c3c81"; };

  };

  class GetUVCExposure {
    public:
    typedef GetUVCExposureRequest Request;
    typedef GetUVCExposureResponse Response;
  };

}
#endif
