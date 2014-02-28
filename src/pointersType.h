#ifndef POINTERSTYPE_H
#define POINTERSTYPE_H
#include <QString>
typedef void (*td_init)(void (*msgOut)(INT8U *buff, INT32U l),
                        void (*sClose)(INT32U),
                        INT32U index);
typedef void (*td_show)();
typedef void (*td_close)();
typedef void (*td_update)(INT8U *buff, INT32U l);
typedef void (*td_changeStyle)(QString style);

#endif // POINTERSTYPE_H
