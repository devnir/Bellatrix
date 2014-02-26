#ifndef _BUFFFUNCTIONS_H_
#define _BUFFFUNCTIONS_H_

#include "typedef.h"


extern C_CPP INT32U store8lu(INT8U *buff, INT8U u8);
extern C_CPP INT32U store8ls(INT8U *buff, INT8S s8);

extern C_CPP INT32U store16lu(INT8U *buff, INT16U u16);
extern C_CPP INT32U store16ls(INT8U *dst, INT16S val);
extern C_CPP INT32U store32lu(INT8U *buff, INT32U u16);
extern C_CPP INT32U store32ls(INT8U *dst, INT32S val);

extern C_CPP INT8U load8lu(INT8U *src);
extern C_CPP INT16U load16lu(INT8U *src);
extern C_CPP INT32U load32lu(INT8U *src);

extern C_CPP INT8S load8ls(INT8U *src);
extern C_CPP INT16S load16ls(INT8U *src);
extern C_CPP INT32S load32ls(INT8U *src);

extern C_CPP INT64S store64ls(INT8U *dst, INT64S val);
#endif
