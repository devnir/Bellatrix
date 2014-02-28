#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logging.h"
#include <QDebug>
#include <QLibrary>

TPlugin plugins[_MAX_PLUGINS_];
bool initPluginPointers(int index);
bool loadLib(int index);
void MainWindow::load_plugins()
{
  // »нициализаци€ структуры описани€ плагинов
  for(INT32U i = 0; i < _MAX_PLUGINS_; i++)
  {
    plugins[i].validity = 0;
    plugins[i].action = new QAction(this);
    ui->menuPlugins->addAction(plugins[i].action);
    plugins[i].action->setCheckable(true);
    plugins[i].action->setEnabled(false);
    plugins[i].action->setVisible(false);
  }
  connect(ui->menuPlugins, SIGNAL(triggered(QAction*)),
          this, SLOT(slotPluginActions(QAction*)));


  // «агрузка инсталированых плагинов
  QSettings set(_PL_NAME_, QSettings::IniFormat);
  int count = set.beginReadArray(_PL_ARR_);
  if(count > _MAX_PLUGINS_)
    count = _MAX_PLUGINS_;

  for(int i = 0; i < count; i++)
  {
    set.setArrayIndex(i);
    plugins[i].validity = set.value(_PLA_ENABLE_, 0).toInt();
    plugins[i].name = set.value(_PLA_NAME_, "NoName").toString();
    plugins[i].fileName = set.value(_PLA_FILE_, "").toString();
    if(plugins[i].validity == 1)
    {
      plugins[i].action->setText(plugins[i].name);
      if(initPluginPointers(i))
      {
        plugins[i].action->setEnabled(true);
        plugins[i].action->setVisible(true);
      }
      else
      {
        plugins[i].validity = 0;
        loging(_LOG_CRIT_, tr("Error on load plugin ") + plugins[i].name);
      }
    }
  }
  set.endArray();
}


void MainWindow::slotPluginActions(QAction *act)
{
  for(int i = 0; i < _MAX_PLUGINS_; i++)
  {
    if(act == plugins[i].action)
    {
      if(plugins[i].validity == 1)
      {
        if(act->isChecked())
        {
          plugins[i].func.show();
        }
        else
        {
          plugins[i].func.close();
        }
      }
    }
  }
}

bool initPluginPointers(int index)
{
  if(index >= _MAX_PLUGINS_)
    return false;

  if(plugins[index].validity != 1)
    return false;

  QFile fs("./plugins/" + plugins[index].fileName);
  if(!fs.exists())
  {
    return false;
  }

  if(!loadLib(index))
  {
    return false;
  }

  return true;
}

void msgOut(INT8U *buff, INT32U l)
{
  int len = Binr2Encode(OutBuff, buff, l);
  if(port->isOpen())
    port->write((char *)OutBuff, len);
}

void sClose(INT32U id)
{
  if(id >= _MAX_PLUGINS_)
    return;

  plugins[id].action->setChecked(false);
}

bool loadLib(int index)
{
  QLibrary lib("./plugins/" + plugins[index].fileName);
  if(!lib.load())
  {
    qDebug() << lib.errorString();
    return false;
  }

  td_init     init = (td_init)lib.resolve("initLib");
  if(!init)
  {
    qDebug() << lib.errorString();
    return false;
  }

  init(msgOut, sClose, index);

  plugins[index].func.show = (td_show)lib.resolve("show");
  if(!plugins[index].func.show)
  {
    qDebug() << lib.errorString();
    return false;
  }

  plugins[index].func.close = (td_close)lib.resolve("close");
  if(!plugins[index].func.close)
  {
    qDebug() << lib.errorString();
    return false;
  }

  plugins[index].func.update = (td_update)lib.resolve("dataUpdate");
  if(!plugins[index].func.update)
  {
    qDebug() << lib.errorString();
    return false;
  }

  plugins[index].func.changeStyle = (td_changeStyle)lib.resolve("changeStyle");
  if(!plugins[index].func.changeStyle)
  {
    qDebug() << lib.errorString();
    return false;
  }

  return true;
}

void MainWindow::updatePluginData()
{
  for(int i = 0; i < _MAX_PLUGINS_; i++)
  {
    if(plugins[i].validity == 1)
      plugins[i].func.update(Binr2DataBuff, Binr2DataSize);
  }
}

void MainWindow::updatePluginStyle(QString style)
{
  for(int i = 0; i < _MAX_PLUGINS_; i++)
  {
    if(plugins[i].validity == 1)
      plugins[i].func.changeStyle(style);
  }
}

