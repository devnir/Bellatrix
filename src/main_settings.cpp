#include "main_settings.h"
#include "defines.h"

MainSettings::MainSettings()
{
}


void MainSettings::load()
{
  QSettings set(_MS_NAME_, QSettings::IniFormat);
  set.beginGroup(_MSG_VERSION_);
  version.major = set.value(_MSK_VERSION_MAJOR_, _VERSION_MAJOR_).toInt();
  version.minor = set.value(_MSK_VERSION_MINOR_, _VERSION_MINOR_).toInt();
  set.endGroup();

  set.beginGroup(_MSG_VIEW_);
  view.QAtoolbarEn = set.value(_MSK_QAT_EN_, _QAT_ENABLE_).toInt();
  view.styleName = set.value(_MSK_STYLE_, _DEFAULT_TEMPLATE_).toString();
  view.QAseriaEn = set.value(_MSK_QAS_EN_,_QAS_ENABLE_).toInt();
  set.endGroup();

  set.beginGroup(_MSG_SERIAL_);
  serial.portName = set.value(_MSK_PNAME_, "").toString();
  serial.baud    = set.value(_MSK_PBAUD_, 115200).toInt();
  set.endGroup();
}

void MainSettings::save()
{
  QSettings set(_MS_NAME_, QSettings::IniFormat);
  set.beginGroup(_MSG_VERSION_);
  set.setValue(_MSK_VERSION_MAJOR_, version.major);
  set.setValue(_MSK_VERSION_MINOR_, version.minor);
  set.endGroup();

  set.beginGroup(_MSG_VIEW_);
  set.setValue(_MSK_QAT_EN_, view.QAtoolbarEn);
  set.setValue(_MSK_QAS_EN_, view.QAseriaEn);
  set.setValue(_MSK_STYLE_, view.styleName);
  set.endGroup();

  set.beginGroup(_MSG_SERIAL_);
  set.setValue(_MSK_PNAME_, serial.portName);
  set.setValue(_MSK_PBAUD_, (int)serial.baud);
  set.endGroup();
}

void MainSettings::toDef()
{
  version.major = _VERSION_MAJOR_;
  version.minor = _VERSION_MINOR_;

  view.QAtoolbarEn = _QAT_ENABLE_;
  view.QAseriaEn = _QAS_ENABLE_;
  view.styleName = _DEFAULT_TEMPLATE_;

  save();
}
