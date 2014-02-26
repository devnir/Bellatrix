#ifndef CRC16_H
#define CRC16_H
#include "typedef.h"

#ifdef __cplusplus
#define C_CPP "C"
#else
#define C_CPP
#endif

extern C_CPP INT16U  crc16CalcByte(INT8U byte, INT16U crc_old);
extern C_CPP INT16U  crc16CalcRaw(INT8U *data, INT16U len);


#endif // CRC16_H
