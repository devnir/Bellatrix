#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <QTimer>
#include <QLabel>
#include <QComboBox>
#include <QToolBar>
#include <QAction>
#include "defines.h"
#include "main_settings.h"
#include "infowidget.h"
#include "config_widget.h"
#include "Binr2.h"
#include "pointersType.h"

typedef struct
{
  td_show        show;
  td_close       close;
  td_update      update;
  td_changeStyle changeStyle;
}TPointerFunc;


typedef struct
{
  INT8U validity;
  QAction *action;
  QString name;
  QString fileName;
  TPointerFunc func;
  TPluginVersion version;
}TPlugin;

namespace Ui {
  class MainWindow;
}

extern INT32U  baudTable[_BAUD_TABLE_SIZE_];
extern TPlugin plugins[_MAX_PLUGINS_];
extern INT8U Binr2DataBuff[];
extern INT32U Binr2DataSize;
extern INT8U   OutBuff[];
extern BINR2_POINTERS b2Ptr;
extern QSerialPort *port;
extern QVector<TPlugin> pluginList;



class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
// Interface


  protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *);

  private slots:
    void on_actionSettings_triggered();
    void on_actionShow_logs_triggered();

    void on_actionSearch_triggered();

    void on_actionConnect_triggered();

    void on_actionDisconnect_triggered();

  public slots:
    void setEnabledQA(bool enabled);
    void setEnabledQAS(bool enabled);
    void changeStyle();

    void slotSearchTimeout();
    void slotSearchDataRead();
    void slotSerialRead();

    void slotPluginActions(QAction *);

    void slotChangeSerialSettings();
  private:
    Ui::MainWindow *ui;
    QComboBox *portBox;
    QComboBox *baudBox;
    void guiInit();
    QLabel *stbLabel[_STATUS_LBL_COUNT_];    

    //WIDGETS
    InfoWidget infoWidg;
    ConfigWidget configWidget;
    void setStatusText(int type, QString str);
    void setPortText(int baud);



    //SYNC
    void searchInit();
    QSerialPort *searchPort;
    QTimer      *searchTimer;
    INT32U       searchIndex;
    void       searchStart();
    void       searchStop();    
    void       searchDone();
    void       avalPorts();
    //Work    
    void updateLocalData();
    void updatePluginData();
    void updatePluginStyle(QString style);
    //plugins
    void load_plugins();


};

#endif // MAINWINDOW_H
