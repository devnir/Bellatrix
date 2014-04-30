#ifndef PLUGINADDDIALOG_H
#define PLUGINADDDIALOG_H

#include <QDialog>

namespace Ui {
  class pluginAddDialog;
}

class pluginAddDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit pluginAddDialog(QWidget *parent = 0);
    ~pluginAddDialog();

  protected:
    void changeEvent(QEvent *e);

  private slots:
    void on_PluginName_textEdited(const QString &arg1);

    void on_browseBtn_clicked();

    void on_okkBtn_clicked();

  private:
    Ui::pluginAddDialog *ui;
  signals:
    void okkPressed(QString filePath, QString pluginName);
};

#endif // PLUGINADDDIALOG_H
