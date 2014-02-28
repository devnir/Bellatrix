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
