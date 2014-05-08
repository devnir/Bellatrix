#ifndef CONFIG_WIDGET_H
#define CONFIG_WIDGET_H

#include <QWidget>
#include <QShowEvent>
#include <QCloseEvent>


#include "main_settings.h"



namespace Ui
{
  class ConfigWidget;
}

class ConfigWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit ConfigWidget(QWidget *parent = 0);    
    ~ConfigWidget();
    void updateData();
    void saveData();


  protected:
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);


  private slots:
    void on_defButton_clicked();
    void on_QABarEnable_stateChanged(int arg1);
    void on_styleBox_currentIndexChanged(int index);
    void on_QASBarEn_stateChanged(int arg1);
    void on_tabWidget_currentChanged(int index);
  signals:
    void signalEnabledQA(bool enabled);
    void signalEnabledQAS(bool enabled);
    void signalStyleChaged();

  private:
    Ui::ConfigWidget *ui;
    MainSettings set;
};

#endif // CONFIG_WIDGET_H
