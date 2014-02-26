#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QLabel>
#include <QComboBox>
#include <QToolBar>
#include "defines.h"
#include "main_settings.h"
#include "infowidget.h"
#include "config_widget.h"


namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
// Interface


  protected:
    void changeEvent(QEvent *e);

  private slots:
    void on_actionSettings_triggered();
    void on_actionShow_logs_triggered();

  public slots:
    void setEnabledQA(bool enabled);
    void changeStyle();

  private:
    Ui::MainWindow *ui;
    QComboBox *portBox;
    QComboBox *baudBox;
    void guiInit();
    QLabel *stbLabel[_STATUS_LBL_COUNT_];

    //WIDGETS
    InfoWidget infoWidg;
    ConfigWidget configWidget;



};

#endif // MAINWINDOW_H
