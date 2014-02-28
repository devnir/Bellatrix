
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "main_settings.h"
#include <math.h>
#include "util.h"
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

  ui->actionDisconnect->setEnabled(false);
  ui->actionDisconnect->setVisible(false);
  ui->actionSettings->setEnabled(true);
  ui->actionShow_logs->setEnabled(true);  
  //WIDGETS
  connect(&configWidget, SIGNAL(signalEnabledQA(bool)),
          this, SLOT(setEnabledQA(bool)));
  connect(&configWidget, SIGNAL(signalEnabledQAS(bool)),
          ui->serialBar, SLOT(setVisible(bool)));

  connect(&configWidget, SIGNAL(signalStyleChaged()),
          this, SLOT(changeStyle()));
  connect(portBox, SIGNAL(currentIndexChanged(int)),
          this, SLOT(slotChangeSerialSettings()));

  connect(baudBox, SIGNAL(currentIndexChanged(int)),
          this, SLOT(slotChangeSerialSettings()));
//
  avalPorts();
//заполнение данных о доступных скоростях обмена.
  int index = 0;
  baudBox->blockSignals(true);
  for(int i = 0; i < _BAUD_TABLE_SIZE_; i++)
  {
    baudBox->addItem(QString("%1").arg(baudTable[i]));
    if(set.serial.baud == baudTable[i])
      index = i;
  }
  baudBox->setCurrentIndex(index);
  baudBox->blockSignals(false);

  setStatusText(_LOG_NORM_, tr("Disconnected"));
  setPortText(baudTable[index]);
}


void MainWindow::setEnabledQA(bool enabled = true)
{
  ui->mainToolBar->setVisible(enabled);
}

void MainWindow::setEnabledQAS(bool enabled = true)
{
  ui->serialBar->setVisible(enabled);
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
    updatePluginStyle(StyleSheet);
    configWidget.setStyleSheet(StyleSheet);
    infoWidg.setStyleSheet(StyleSheet);
  }
  else
  {
    this->setStyleSheet("");
    configWidget.setStyleSheet("");
    infoWidg.setStyleSheet("");
    loging(_LOG_CRIT_, "Style load error\r\n");
  }

  ui->actionSearch->setIcon(QIcon("./template/" + styleName + "/images/search.png"));
  ui->actionSettings->setIcon(QIcon("./template/" + styleName + "/images/config.png"));
  ui->actionShow_logs->setIcon(QIcon("./template/" + styleName + "/images/info.png"));
  ui->actionConnect->setIcon(QIcon("./template/" + styleName + "/images/connect.png"));
  ui->actionDisconnect->setIcon(QIcon("./template/" + styleName + "/images/disconnect.png"));
}

void MainWindow::setStatusText(int type, QString str)
{
  QString out;
  out = colorStr[type] + str + endStr;
  stbLabel[0]->setText(out);
}

void MainWindow::setPortText(int baud)
{
  MainSettings set;
  set.load();
  stbLabel[1]->setText(portBox->currentText() + " " + QString("%1").arg(baud));
}

void MainWindow::updateLocalData()
{
  if(b2Ptr.p90 == NULL)
    return;
  QString str;
  double b = (double)b2Ptr.p90->b / 1e7 * M_PI / 180.;
  double l = (double)b2Ptr.p90->l / 1e7 * M_PI / 180.;
  //str.sprintf("%s", LatitudeString(b));
  str.sprintf("H: %+08.2lf", b2Ptr.p90->h / 100.);

  ui->b_label->setText(QString(LatitudeString(b)));
  ui->L_label->setText(QString(LongitudeString(l)));
  ui->H_label->setText(str);

  float rms = sqrt((b2Ptr.p90->bRms / 10.) * (b2Ptr.p90->bRms / 10.) +
                   (b2Ptr.p90->lRms / 10.) * (b2Ptr.p90->lRms / 10.));

  if(rms <= 999.9)
   str.sprintf("rms2D: %.1f", rms);
  else
   str.sprintf("rms2D: ****");

  ui->RMS_label->setText(str);

  float Vb = b2Ptr.p90->Vb / 100.;
  float Vl = b2Ptr.p90->Vl / 100.;
  float speed = sqrt(Vb * Vb + Vl * Vl);
  float course = 0.;

  if((Vl != 0.) || (Vb != 0.))
  {
   course = atan2(Vl, Vb) * 180. / M_PI;
   if(course < 0.)
    course += 360.;
  }

  str.sprintf("Speed: %05.1f km/h", speed * 3.6);
  ui->speed_label->setText(str);
  str.sprintf("Course:%05.1f \xC2\xB0", course);
  ui->course_label->setText(str);

  if(b2Ptr.p90->HDOP <= 100)
   str.sprintf("HDOP:  %.1f", b2Ptr.p90->HDOP / 10.);
  else
   str.sprintf("HDOP:  ****");
  ui->hdop_label->setText(str);
  if(b2Ptr.p90->VDOP <= 100)
   str.sprintf("VDOP:  %.1f", b2Ptr.p90->VDOP / 10.);
  else
   str.sprintf("VDOP:  ****");
  ui->vdop_label->setText(str);

  ui->GPS_label->setText(str.sprintf("GPS: %02d", b2Ptr.p90->nGPS));
  ui->GLN_label->setText(str.sprintf("GLN: %02d", b2Ptr.p90->nGLN));
  ui->GLL_label->setText(str.sprintf("GLL: %02d", b2Ptr.p90->nGLL));
}

