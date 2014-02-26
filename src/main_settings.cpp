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
  set.setValue(_MSK_STYLE_, view.styleName);
  set.endGroup();
}

void MainSettings::toDef()
{
  version.major = _VERSION_MAJOR_;
  version.minor = _VERSION_MINOR_;

  view.QAtoolbarEn = _QAT_ENABLE_;
  view.styleName = _DEFAULT_TEMPLATE_;

  save();
}
