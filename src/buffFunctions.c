#include "buffFunctions.h"

/******************************************************************************
* Function Definitions
******************************************************************************/
INT32U store8lu(INT8U *buff, INT8U u8)
{
  buff[0] = u8;
  return 1;
}
/******************************************************************************
* OVERVIEW :
* INPUT    :
* OUTPUT   :
* RETURN   :
******************************************************************************/
INT32U store8ls(INT8U *buff, INT8S s8)
{
  buff[0] = s8&0xFF;
  return 1;
}
/******************************************************************************
* OVERVIEW :
* INPUT    : 
* OUTPUT   :
* RETURN   : 
******************************************************************************/
INT32U store16lu(INT8U *dst, INT16U val)
{
  register unsigned char *p = (unsigned char *)(dst);
  register unsigned v = (unsigned)(val);
  p[0] = (unsigned char) (v & 0xFF);
  p[1] = (unsigned char) ((v >> 8) & 0xFF);
  return 2;
}
/******************************************************************************
* OVERVIEW :
* INPUT    : 
* OUTPUT   :
* RETURN   : 
******************************************************************************/
INT32U store16ls(INT8U *dst, INT16S val)
{
  register int v = (int)(val); 
  register unsigned char *p = (unsigned char *)(dst);
  if (v < 0)
  {
    v = (int) (((-v - 1) & 0x7FFF) ^ 0x7FFF); 
    p[1] = (unsigned char) (0x80 | ((v >> 8) & 0x7F)); 
  }
  else   
    p[1] = (unsigned char) ((v >> 8) & 0x7F);
  p[0] = (unsigned char) (v & 0xFF);
  return 2;
}
/******************************************************************************
* OVERVIEW :
* INPUT    : 
* OUTPUT   :
* RETURN   : 
******************************************************************************/
INT32U store32lu(INT8U *dst, INT32U val)
{
  register unsigned char *p = (unsigned char *)(dst); 
  register unsigned long v = (unsigned long)(val); 
  p[0] = (unsigned char) (v & 0xFF);
  p[1] = (unsigned char) ((v >> 8) & 0xFF);
  p[2] = (unsigned char) ((v >> 16) & 0xFF);
  p[3] = (unsigned char) ((v >> 24) & 0xFF);
  return 4;
}
/******************************************************************************
* OVERVIEW :
* INPUT    : 
* OUTPUT   :
* RETURN   : 
******************************************************************************/
INT64S store64ls(INT8U *p, INT64S val)
{
  p[0] = (unsigned char) (val & 0xFF);
  p[1] = (unsigned char) ((val >> 8) & 0xFF);
  p[2] = (unsigned char) ((val >> 16) & 0xFF);
  p[3] = (unsigned char) ((val >> 24) & 0xFF);
  p[4] = (unsigned char) ((val >> 32) & 0xFF);
  p[5] = (unsigned char) ((val >> 40) & 0xFF);
  p[6] = (unsigned char) ((val >> 48) & 0xFF);
  p[7] = (unsigned char) ((val >> 56) & 0xFF);
  return 8;
}
/******************************************************************************
* OVERVIEW :
* INPUT    : 
* OUTPUT   :
* RETURN   : 
******************************************************************************/
INT32U store32ls(INT8U *dst, INT32S val)
{
  register long v = (long)(val);
  register unsigned char *p = (unsigned char *)(dst);
  if (v < 0) 
  {
    v = (long) (((-v - 1) & 0x7FFFFFFFL) ^ 0x7FFFFFFFL);
    p[3] = (unsigned char) (0x80 | ((v >> 24) & 0x7F)); 
  }
  else
  {
    p[3] = (unsigned char) ((v >> 24) & 0x7F); 
  }
  p[0] = (unsigned char) (v & 0xFF); 
  p[1] = (unsigned char) ((v >> 8) & 0xFF);
  p[2] = (unsigned char) ((v >> 16) & 0xFF);   
  return 4;  
}

/******************************************************************************
* OVERVIEW :
* INPUT    : 
* OUTPUT   :
* RETURN   : 
******************************************************************************/
INT8U load8lu(INT8U *src)
{
  return ((unsigned)((((const unsigned char*)(src))[0] & 0xFF)));
}
/******************************************************************************
* OVERVIEW :
* INPUT    : 
* OUTPUT   :
* RETURN   : 
******************************************************************************/
INT16U load16lu(INT8U *src)
{
  return  ((unsigned)((((const unsigned char*)(src))[0] & 0xFF) | (((unsigned)*((const unsigned char*)(src)+1) & 0xFF) << 8)));
}
/******************************************************************************
* OVERVIEW :
* INPUT    : 
* OUTPUT   :
* RETURN   : 
******************************************************************************/
INT32U load32lu(INT8U *src) 
{
  INT32U ret =
           ((unsigned long)((((const unsigned char*)(src))[0] & 0xFF)
        | (((unsigned long)*((const unsigned char*)(src)+1) & 0xFF) << 8)
        | (((unsigned long)*((const unsigned char*)(src)+2) & 0xFF) << 16)
        | (((unsigned long)*((const unsigned char*)(src)+3) & 0xFF) << 24)));
  return ret;
}
/******************************************************************************
* OVERVIEW :
* INPUT    : 
* OUTPUT   :
* RETURN   : 
******************************************************************************/
INT8S load8ls(INT8U *src)
{
  INT8S ret =     ((int)((((const unsigned char*)(src))[0] & 0x80) ? 
           -((((const unsigned char*)(src))[0] & 0x7F) ^ 0x7F) - 1 : 
           ((const unsigned char*)(src))[0] & 0x7F));
  return ret;
}
/******************************************************************************
* OVERVIEW :
* INPUT    : 
* OUTPUT   :
* RETURN   : 
******************************************************************************/
INT16S load16ls(INT8U *src)
{
  INT16S ret =      ((int)(((const unsigned char*)(src))[1] & 0x80 ? -(( 
           (((const unsigned char*)(src))[0] & 0xFF) * 1 + 
           (((const unsigned char*)(src))[1] & 0x7F) * 256 
       ) ^ 32767) - 1 : ( 
           + (((const unsigned char*)(src))[0] & 0xFF) * 1 
           + (((const unsigned char*)(src))[1] & 0xFF) * 256 
       )
    ));
  return ret;
}
/******************************************************************************
* OVERVIEW :
* INPUT    : 
* OUTPUT   :
* RETURN   : 
******************************************************************************/
INT32S load32ls(INT8U *src)
{
  INT32S ret = ((long)(((const unsigned char*)(src))[3] & 0x80 ? -(( 
           (((const unsigned char*)(src))[0] & 0xFF) * 1 + 
           (((const unsigned char*)(src))[1] & 0xFF) * 256L + 
           (((const unsigned char*)(src))[2] & 0xFF) * 65536L + 
           (((const unsigned char*)(src))[3] & 0x7F) * 16777216L 
       ) ^ 2147483647L) - 1 : ( 
           + (((const unsigned char*)(src))[0] & 0xFF) * 1 
           + (((const unsigned char*)(src))[1] & 0xFF) * 256 
           + (((const unsigned char*)(src))[2] & 0xFF) * 65536L 
           + (((const unsigned char*)(src))[3] & 0xFF) * 16777216L 
       )
    ));
  return ret;
}
