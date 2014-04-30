#ifndef PLUGINEDITDIALOG_H
#define PLUGINEDITDIALOG_H

#include <QDialog>

namespace Ui {
  class pluginEditDialog;
}

class pluginEditDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit pluginEditDialog(QWidget *parent = 0);
    ~pluginEditDialog();

  protected:
    void changeEvent(QEvent *e);

  private:
    Ui::pluginEditDialog *ui;
};

#endif // PLUGINEDITDIALOG_H
