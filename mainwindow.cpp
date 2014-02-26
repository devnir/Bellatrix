#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logging.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  log(_LOG_GOOD_, "Runing app...");
  ui->setupUi(this);
  guiInit();
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

