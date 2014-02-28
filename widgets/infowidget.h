#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QTabWidget>
#include <QEvent>

namespace Ui {
  class InfoWidget;
}

class InfoWidget : public QTabWidget
{
    Q_OBJECT

  public:
    explicit InfoWidget(QWidget *parent = 0);
    ~InfoWidget();

  protected:
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent *);

  private:
    Ui::InfoWidget *ui;

};

#endif // INFOWIDGET_H
