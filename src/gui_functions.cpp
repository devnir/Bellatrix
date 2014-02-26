#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "main_settings.h"
#include "logging.h"


void MainWindow::guiInit()
{
  MainSettings set;
  set.load();
  this->setWindowTitle(_APP_NAME_ + QString(" v%1.%2").arg(
                         set.version.major).arg(set.version.minor));

  for(INT32U i = 0; i < _STATUS_LBL_COUNT_; i++)
  {
    stbLabel[i] = new QLabel;
    this->ui->statusBar->addWidget(stbLabel[i]);
    stbLabel[i]->setText(QString("Status Label %1").arg(i));
  }  



  portBox = new QComboBox();
  baudBox = new QComboBox();
  ui->serialBar->addWidget(portBox);
  ui->serialBar->addWidget(baudBox);
  ui->serialBar->setVisible(false);

  changeStyle();
  ui->actionConnect->setEnabled(false);
  ui->actionDisconnect->setEnabled(false);
  ui->actionSearch->setEnabled(false);
  ui->actionSettings->setEnabled(true);
  ui->actionShow_logs->setEnabled(true);
  //WIDGETS
  connect(&configWidget, SIGNAL(signalEnabledQA(bool)),
          this, SLOT(setEnabledQA(bool)));

  connect(&configWidget, SIGNAL(signalStyleChaged()),
          this, SLOT(changeStyle()));


}

void MainWindow::setEnabledQA(bool enabled = true)
{
  ui->mainToolBar->setVisible(enabled);
}

void MainWindow::changeStyle()
{
  MainSettings set;
  set.load();
  QString styleName = set.view.styleName;

  QFile File("./template/" + styleName + "/"+styleName+".qss");
  if(File.open(QFile::ReadOnly))
  {
    QString StyleSheet = QLatin1String(File.readAll());
    this->setStyleSheet(StyleSheet);
  }
  else
  {
    this->setStyleSheet("");
    log(_LOG_CRIT_, "Style load error\r\n");
  }

  ui->actionSearch->setIcon(QIcon("./template/" + styleName + "/images/search.png"));
  ui->actionSettings->setIcon(QIcon("./template/" + styleName + "/images/config.png"));
  ui->actionShow_logs->setIcon(QIcon("./template/" + styleName + "/images/info.png"));
  ui->actionConnect->setIcon(QIcon("./template/" + styleName + "/images/connect.png"));
  ui->actionDisconnect->setIcon(QIcon("./template/" + styleName + "/images/disconnect.png"));
}


