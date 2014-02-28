#include "logging.h"

QStringList logList;

QString colorStr[3] =
{
  "<font color = 'red'>",
  "<font color = 'green'>",
  "<font color = 'blue'>",
};

QString endStr = "</font>";


QString loging(int type, QString str)
{
  QString out;
  out = colorStr[type] + str + endStr;
  logList.append(out);
  return out;
}

QStringList getLogs()
{
  return logList;
}
