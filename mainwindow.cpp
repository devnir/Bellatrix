#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logging.h"
BINR2_POINTERS b2Ptr;
BINR2x80 px80;
BINR2x90 px90;
BINR2xC0 pxC0;

INT8U Binr2DataBuff[4096];
INT32U Binr2DataSize = 0;
QSerialPort *port;

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
  Binr2Init(b2Ptr);  
  guiInit();
  load_plugins();
  changeStyle();
  searchInit();

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
    ui->menuPlugins->setEnabled(false);
    searchStart();
}

void MainWindow::closeEvent(QCloseEvent *)
{
  /*
  for(int i = 0; i < _MAX_PLUGINS_; i++)
  {
    if(plugins[i].validity)
    {
      if(plugins[i].action->isChecked())
      {
        plugins[i].func.close();
      }
    }
  }*/
}



