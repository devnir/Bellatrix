#ifndef DEFINES_H
#define DEFINES_H

#define _APP_NAME_          "Bellatrix"
#define _VERSION_MAJOR_     0
#define _VERSION_MINOR_     1
#define _QAT_ENABLE_        1
#define _QAS_ENABLE_        1
#define _DEFAULT_TEMPLATE_  "default"

#define _BAUD_TABLE_SIZE_   4
#define _MAX_PLUGINS_       32



//--GUI
#define _STATUS_LBL_COUNT_  5
//MS = Main Settings
//MSG - Main Settings Group
//MSK - Main Settings Key
#define _MS_NAME_           "global.dat"
#define _MSG_VERSION_       "version"
#define _MSK_VERSION_MAJOR_ "major"
#define _MSK_VERSION_MINOR_ "minor"
//--VIEW
#define _MSG_VIEW_          "view"
#define _MSK_QAT_EN_        "qat_en"
#define _MSK_QAS_EN_        "qas_en"
#define _MSK_STYLE_         "style_name"
//SERIAL
#define _MSG_SERIAL_        "serial"
#define _MSK_PNAME_         "port_name"
#define _MSK_PBAUD_         "port_baud"
//PLUGINS

#define _PL_NAME_           "plugins.dat"
#define _PL_ARR_            "plugins"
#define _PLA_NAME_          "name"
#define _PLA_FILE_          "file"
#define _PLA_ENABLE_        "enable"
//LOGGING
#define _LOG_CRIT_    0
#define _LOG_GOOD_    1
#define _LOG_NORM_    2

#endif // DEFINES_H
