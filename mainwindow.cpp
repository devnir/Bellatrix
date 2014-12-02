#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logging.h"
#include "QFileDialog"
#include <QDebug>
BINR2_POINTERS b2Ptr;
BINR2x80 px80;
BINR2x90 px90;
BINR2x92 px92;
BINR2xC0 pxC0;

INT8U Binr2DataBuff[4096];
INT32U Binr2DataSize = 0;
quint64   logFileSize = 0;
QSerialPort *port;
QSerialPort *searchPort;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  loging(_LOG_GOOD_, "Runing app...");
  ui->setupUi(this);

  b2Ptr.dataBuff = Binr2DataBuff;
  b2Ptr.dataLen = &Binr2DataSize;
  b2Ptr.p80 = &px80;
  b2Ptr.p90 = &px90;
  b2Ptr.pC0 = &pxC0;
  b2Ptr.p92 = &px92;
  Binr2Init(b2Ptr);  
  guiInit();
  load_plugins();
  changeStyle();
  searchInit();
  logFileOffset = 0;
  logReadTimer = new QTimer(this);
  logReadTimer->setSingleShot(true);
  connect(logReadTimer, SIGNAL(timeout()), this, SLOT(logFileReadimeout())); 
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
  QMainWindow::changeEvent(e);
  switch (e->type())
  {
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
    break;
    default:
    break;
  }
}

void MainWindow::on_actionSettings_triggered()
{
  configWidget.show();
}

void MainWindow::on_actionShow_logs_triggered()
{
    infoWidg.show();
}


void MainWindow::on_actionSearch_triggered()
{
    ui->actionSearch->setEnabled(false);
    ui->actionConnect->setEnabled(false);
    ui->serialBar->setEnabled(false);
    searchStart();
}

void MainWindow::closeEvent(QCloseEvent *)
{  
  for(int i = 0; i < pluginList.count(); i++)
  {
    if(pluginList.at(i).action->isChecked())
    {
      pluginList.at(i).func.close();
    }
  }
}


void MainWindow::on_browseToolButton_clicked()
{
  QDir dir("./");
  dir.mkdir("_Files_");
  QFileDialog *fd = new QFileDialog(this);
  fd->setFileMode(QFileDialog::AnyFile);
  fd->setDirectory("./_Files_/");
  if(fd->exec())
  {
    logFileInfo.setFile(fd->selectedFiles().at(0));
    QString str("File: ");
    str += logFileInfo.fileName();
    ui->fileNameLbl->setText(str);
    ui->fileSize->setText(fileSizeToStr(logFileInfo.size()));    
  }
}

void MainWindow::on_recordToolButton_clicked()
{
  QDir dir("./");
  dir.mkdir("_Files_");
  if(ui->recordToolButton->isChecked())
  {
    logFileSize = 0;
    ui->fileStatus->setText("State: recording");
    if(logFileInfo.fileName().isEmpty())
    {
      logFileInfo.setFile("./_Files_/default.b2");      
    }
    logFile.setFileName(logFileInfo.filePath());
    logFile.open(QIODevice::ReadWrite);
    logFile.close();
    QString str("File: ");
    str += logFileInfo.fileName();
    ui->fileNameLbl->setText(str);
    ui->fileSize->setText(fileSizeToStr(logFileSize));
  }
  else
  {
    logFile.close();
    ui->fileStatus->setText("State: stop");
    ui->fileSize->setText(fileSizeToStr(logFileSize));
  }
}

void MainWindow::on_playToolButton_clicked()
{
  if(logFile.isOpen())
  {
    logFile.close();
  }
  QString str("File: ");
  str += logFileInfo.fileName();
  ui->fileNameLbl->setText(str);
  ui->fileSize->setText(fileSizeToStr(logFileInfo.size()));

  if(ui->playToolButton->isChecked())
  {
    logReadTimer->setInterval(ui->intervalLcd->value());
    QString str;
    str.sprintf("State: read");
    ui->fileStatus->setText(str);
    logReadTimer->start();
  }
  else
  {
    logReadTimer->stop();
    QString str;
    str.sprintf("State: pause");
    ui->fileStatus->setText(str);
  }
}

void MainWindow::logFileReadimeout()
{
  QFile readFile;
  readFile.setFileName(logFileInfo.filePath());
  QByteArray buff;
  quint64 size = 0;
  QString str;
  if(readFile.open(QIODevice::ReadOnly))
  {
    readFile.seek(logFileOffset);
    buff = readFile.read(_READ_SEGMEN_SIZE_);
    size = readFile.size();
    readFile.close();
  }
  else
  {
    str.clear();
    str.sprintf("State: read err");
    ui->fileStatus->setText(str);
    return;
  }
  for(int i = 0; i < buff.length(); i++)
  {
    int ret = Binr2Unpack(buff.at(i) & 0xFF);
    if(ret != 0)
    {
      logFileOffset += i;
      updateLocalData();
      updatePluginData();
      logReadTimer->setInterval(ui->intervalLcd->value());
      logReadTimer->start();
      str.clear();
      str.sprintf("State: read %d\%", int(100./size * logFileOffset));
      ui->fileStatus->setText(str);
      return;
    }
  }
  str.clear();
  logFileOffset += buff.length();
  str.sprintf("State: read %d\%", int(100./size * logFileOffset));
  ui->fileStatus->setText(str);
  if(logFileOffset >= size)
  {
    str.sprintf("State: read done");
    ui->fileStatus->setText(str);
    return;
  }
  logReadTimer->setInterval(ui->intervalLcd->value());
  logReadTimer->start();
}

void MainWindow::on_stopToolButton_clicked()
{
  logReadTimer->stop();
  logFileOffset = 0;
  ui->playToolButton->setChecked(false);
  if(ui->recordToolButton->isChecked())
  {
    ui->recordToolButton->setChecked(false);
    logFile.close();
  }
}
