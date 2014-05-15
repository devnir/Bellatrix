#ifndef UTIL_H
#define UTIL_H
#include "typedef.h"
#include <QString>
extern char* LatitudeString(double b);
extern char* LongitudeString(double l);
extern char* clockString(int t);
extern char* dateString(int t, int w);
extern QString fileSizeToStr(quint64 size);
#endif // UTIL_H
