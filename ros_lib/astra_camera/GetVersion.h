#ifndef _ROS_SERVICE_GetVersion_h
#define _ROS_SERVICE_GetVersion_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace astra_camera
{

static const char GETVERSION[] = "astra_camera/GetVersion";

  class GetVersionRequest : public ros::Msg
  {
    public:

    GetVersionRequest()
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

    virtual const char * getType() override { return GETVERSION; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class GetVersionResponse : public ros::Msg
  {
    public:
      typedef const char* _version_type;
      _version_type version;
      typedef const char* _core_version_type;
      _core_version_type core_version;

    GetVersionResponse():
      version(""),
      core_version("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      uint32_t length_version = strlen(this->version);
      varToArr(outbuffer + offset, length_version);
      offset += 4;
      memcpy(outbuffer + offset, this->version, length_version);
      offset += length_version;
      uint32_t length_core_version = strlen(this->core_version);
      varToArr(outbuffer + offset, length_core_version);
      offset += 4;
      memcpy(outbuffer + offset, this->core_version, length_core_version);
      offset += length_core_version;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      uint32_t length_version;
      arrToVar(length_version, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_version; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_version-1]=0;
      this->version = (char *)(inbuffer + offset-1);
      offset += length_version;
      uint32_t length_core_version;
      arrToVar(length_core_version, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_core_version; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_core_version-1]=0;
      this->core_version = (char *)(inbuffer + offset-1);
      offset += length_core_version;
     return offset;
    }

    virtual const char * getType() override { return GETVERSION; };
    virtual const char * getMD5() override { return "9dcb65d43afc4a7688056ebe631dc337"; };

  };

  class GetVersion {
    public:
    typedef GetVersionRequest Request;
    typedef GetVersionResponse Response;
  };

}
#endif
