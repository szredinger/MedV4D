#ifndef NETSTREAM_IMPL_H
#define NETSTREAM_IMPL_H

#ifdef __unix__
#include <arpa/inet.h>
#endif

#include <vector>

#include "netStream.h"

namespace M4D
{
namespace CellBE
{

  /**
   *  Actual implementation of NetStream interface. In this class are 
   *  serializing and deserializing operators of some common data types
   *  implemented. Also contains pure virtual functions for byte 
   *  putting and retrival alowing different implementation in successors.
   */
class NetStreamBaseImpl : public NetStream
{
protected:
  void virtual AddByte( uint8) = 0;
  uint8 virtual GetByte(void) = 0;

  uint16 supp16;
  uint32 supp32;

  uint8 *ptr8;

public:
  NetStream & operator<< (const uint8 what)
  {
    AddByte( what);
    return *this;
  }

  NetStream & operator<< (const uint16 what)
  {
    supp16 = htons(what);  // convert it to network representation

    AddByte( ((uint8*)&supp16)[0] );
    AddByte( ((uint8*)&supp16)[1] );
    return *this;
  }

  NetStream & operator<< (const uint32 what)
  {
    supp32 = htonl(what);  // convert it to network representation

    AddByte( ((uint8*)&supp32)[0] );
    AddByte( ((uint8*)&supp32)[1] );
    AddByte( ((uint8*)&supp32)[2] );
    AddByte( ((uint8*)&supp32)[3] );
    return *this;
  }

  NetStream & operator<< (const float32 what)
  {
    supp32 = htonl(*((uint32*)&what));  // convert it to network representation

    AddByte( ((uint8*)&supp32)[0] );
    AddByte( ((uint8*)&supp32)[1] );
    AddByte( ((uint8*)&supp32)[2] );
    AddByte( ((uint8*)&supp32)[3] );
    return *this;
  }

  NetStream & operator>>( uint8 &what)
  {
    what = GetByte();
    return *this;
  }

  NetStream & operator>>( uint16 &what)
  {
    ptr8 = (uint8*)&supp16;

    ptr8[0] = GetByte();
    ptr8[1] = GetByte();
    what = ntohs( supp16);

    return *this;
  }

  NetStream & operator>>( uint32 &what)
  {
    ptr8 = (uint8*)&supp32;
    ptr8[0] = GetByte();
    ptr8[1] = GetByte();
    ptr8[2] = GetByte();
    ptr8[3] = GetByte();

    what = ntohl( supp32);
    
    return *this;
  }

  NetStream & operator>>( float32 &what)
  {
    ptr8 = (uint8*)&supp32;
    ptr8[0] = GetByte();
    ptr8[1] = GetByte();
    ptr8[2] = GetByte();
    ptr8[3] = GetByte();

    supp32 = ntohl( *((uint32*)&supp32));
    what = *( (float32*)&supp32);

    return *this;
  }
};

///////////////////////////////////////////////////////////////////////////////

/**
 *  NetStream based on memory buffer. It manages underlaying buffer
 *  interpreted as an array of bytes.
 */
class NetStreamArrayBuf : public NetStreamBaseImpl
{
  uint8 *begin; // underlaying buf
  uint8 *curr;
  uint8 *end;

public:

  NetStreamArrayBuf( uint8 *buf, size_t size)
  {
    begin = buf;
    curr = buf;
    end = buf + size; 
  }

  void AddByte( uint8 what)
  {
    if( curr != end)
    {
      *curr = what;
      curr++;
    }
  }

  uint8 GetByte( void)
  {
    if( curr != end)
      return *curr++;
    else
      throw ExceptionBase("Already on end of stream");
  }

};

///////////////////////////////////////////////////////////////////////////////

/**
 *  NetStream based on underlaying std::vector of bytes.
 */
class NetStreamVector 
  : public std::vector<uint8>, public NetStreamBaseImpl
{
  size_t curr;
public:

  NetStreamVector() : curr(0) {}

  void AddByte( uint8 what)
  {
    push_back( what);
  }

  uint8 GetByte( void)
  {
    if( curr < size())
      return this->at(curr++);
    else
      throw ExceptionBase("Already on end of stream");
  }

};

}
}

#endif
