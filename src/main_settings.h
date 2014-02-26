#ifndef MAIN_SETTINGS_H
#define MAIN_SETTINGS_H
#include <QSettings>
#include "typedef.h"


typedef struct
{
    INT8U major;
    INT8S minor;
}TVersion;

typedef struct
{
  QString   styleName;
  INT8U     QAtoolbarEn;
}TView;
class MainSettings
{
  public:
    MainSettings();
    void load();
    void save();
    void toDef();
    //Version
    TVersion version;
    TView view;
};

#endif // MAIN_SETTINGS_H
