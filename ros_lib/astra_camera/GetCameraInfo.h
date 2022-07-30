#ifndef _ROS_SERVICE_GetCameraInfo_h
#define _ROS_SERVICE_GetCameraInfo_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "sensor_msgs/CameraInfo.h"

namespace astra_camera
{

static const char GETCAMERAINFO[] = "astra_camera/GetCameraInfo";

  class GetCameraInfoRequest : public ros::Msg
  {
    public:

    GetCameraInfoRequest()
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

    virtual const char * getType() override { return GETCAMERAINFO; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class GetCameraInfoResponse : public ros::Msg
  {
    public:
      typedef sensor_msgs::CameraInfo _info_type;
      _info_type info;

    GetCameraInfoResponse():
      info()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->info.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->info.deserialize(inbuffer + offset);
     return offset;
    }

    virtual const char * getType() override { return GETCAMERAINFO; };
    virtual const char * getMD5() override { return "1802ea5c04df755ec7e68bc4bf07a06d"; };

  };

  class GetCameraInfo {
    public:
    typedef GetCameraInfoRequest Request;
    typedef GetCameraInfoResponse Response;
  };

}
#endif
