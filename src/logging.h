#ifndef LOGGING_H
#define LOGGING_H
#include "typedef.h"
#include "defines.h"
#include <QStringList>



extern C_CPP QString log(int type, QString str);
extern C_CPP QStringList getLogs();




#endif // LOGGING_H
