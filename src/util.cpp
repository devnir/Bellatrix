#include "util.h"
#include <math.h>
#include <string.h>
#include <stdio.h>



char* LatitudeString(double b)
{
 static char latitude[80];
 char latchar;
 double degree = fabs(b * 180. / M_PI);

 if(b >= 0.)
  latchar = 'N';
 else
  latchar = 'S';
 sprintf(latitude, "B:  %02d\xC2\xB0%08.5lf%c", (int)degree, (degree - (int)degree) * 60, latchar);
 return latitude;
}

char* LongitudeString(double l)
{
 static char longitude[80];
 char lonchar;
 double degree = fabs(l * 180. / M_PI);

 if(l >= 0.)
  lonchar = 'E';
 else
  lonchar = 'W';
 sprintf(longitude, "L: %03d\xC2\xB0%08.5lf%c", (int)degree, (degree - (int)degree) * 60, lonchar);
 return longitude;
}

char* clockString(int t)
{
 int hh, mm, ss;
 static char clock[10];

 t %= 86400;
 hh = (int)(t / 3600);
 mm = (int)((t % 3600) / 60);
 ss = t % 60;
 sprintf(clock, "%02d:%02d:%02d", hh, mm, ss);
 return clock;
}

//---------------------------------------------------------------------------
static short MDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

char* dateString(int t, int w)
{
 int dd, mm, yy;
 static char date[10];

 int weekday = (int)(t / 86400);
 int i = 0;
 int j = 0;
 int numyear, quartnum;

 w += 1024;
 weekday += w * 7 + 5;
 quartnum = (int)(weekday / 1461);
 weekday -= quartnum * 1461;
 if(weekday < 366)
 {
  numyear  = 0;
  MDays[1] = 29;
 }
 else
 {
  weekday -= 366;
  numyear = weekday / 365 + 1;
  weekday -= (numyear - 1) * 365;
  MDays[1] = 28;
 }
 yy = 1980 + quartnum * 4 + numyear;
 while (i <= weekday)
 {
  i += MDays[j];
  j++;
 };
 mm = j;
 dd = weekday - i + MDays[j-1] + 1;

 sprintf(date, "%02d.%02d.%02d", dd, mm, yy % 100);
 return date;
}

