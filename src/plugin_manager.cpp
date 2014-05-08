#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logging.h"
#include <QDebug>
#include <QLibrary>
#include <QVector>
#include <QDirIterator>
#include <QFileInfoList>

bool checkPlugin(QString fileName);
bool loadLib(QString fileName, TPlugin *plugin, int index);
QVector<TPlugin> pluginList;

void MainWindow::load_plugins()
{
  // »нициализаци€ структуры описани€ плагинов
  connect(ui->menuPlugins, SIGNAL(triggered(QAction*)),
          this, SLOT(slotPluginActions(QAction*)));

  QDirIterator dirIt("./plugins/",QDirIterator::Subdirectories);
  while (dirIt.hasNext())
  {
    dirIt.next();
    if (QFileInfo(dirIt.filePath()).isFile())
      if (QFileInfo(dirIt.filePath()).suffix() == "dll")
      {
        if(checkPlugin(dirIt.filePath()))
        {
          TPlugin plugin;
          if(loadLib(dirIt.filePath(), &plugin, pluginList.size()))
          {
            qDebug() << dirIt.filePath();            
            plugin.action = new QAction(this);
            plugin.fileName = dirIt.fileName();
            plugin.action->setText(plugin.version.name);
            ui->menuPlugins->addAction(plugin.action);
            plugin.action->setCheckable(true);
            plugin.action->setEnabled(true);
            plugin.action->setVisible(true);
            pluginList.append(plugin);
          }
        }
      }
  }
}


void MainWindow::slotPluginActions(QAction *act)
{
  for(int i = 0; i < pluginList.size(); i++)
  {    
    if(act == pluginList.at(i).action)
    {
        if(act->isChecked())
        {
          pluginList.at(i).func.show();
        }
        else
        {
          pluginList.at(i).func.close();
        }      
    }
  }
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

  pluginList.at(id).action->setChecked(false);
}

bool checkPlugin(QString fileName)
{
  QLibrary lib(fileName);
  if(!lib.load())
  {
    qDebug() << lib.errorString();
    return false;
  }

  td_getVersion version = (td_getVersion)lib.resolve("getVersion");
  if(!version)
  {
    qDebug() << lib.errorString();
    return false;
  }
  TPluginVersion pl;
  version(&pl);

  if(pl.major !=_VERSION_MAJOR_ )
  {
    qDebug() << "Major err";
    return false;
  }
  if(pl.minor > _VERSION_MINOR_)
  {
    qDebug() << "Minor err";
    return false;    
  }
  return true;
}

bool loadLib(QString fileName, TPlugin *plugin, int index)
{  
  QLibrary lib(fileName);

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

  td_getVersion version = (td_getVersion)lib.resolve("getVersion");
  if(!version)
  {
    qDebug() << lib.errorString();
    return false;
  }

  version(&plugin->version);

  plugin->func.show = (td_show)lib.resolve("show");
  if(!plugin->func.show)
  {
    qDebug() << lib.errorString();
    return false;
  }

  plugin->func.close = (td_close)lib.resolve("close");
  if(!plugin->func.close)
  {
    qDebug() << lib.errorString();
    return false;
  }

  plugin->func.update = (td_update)lib.resolve("dataUpdate");
  if(!plugin->func.update)
  {
    qDebug() << lib.errorString();
    return false;
  }

  plugin->func.changeStyle = (td_changeStyle)lib.resolve("changeStyle");
  if(!plugin->func.changeStyle)
  {
    qDebug() << lib.errorString();
    return false;
  }  
  return true;
}

void MainWindow::updatePluginData()
{
  for(int i = 0; i < pluginList.size(); i++)
  {    
      pluginList.at(i).func.update(Binr2DataBuff, Binr2DataSize);
  }
}

void MainWindow::updatePluginStyle(QString style)
{
  for(int i = 0; i < pluginList.size(); i++)
  {
    pluginList.at(i).func.changeStyle(style);
  }
}

