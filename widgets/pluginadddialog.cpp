#include "pluginadddialog.h"
#include "ui_pluginadddialog.h"
#include <QFileDialog>

pluginAddDialog::pluginAddDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::pluginAddDialog)
{
  ui->setupUi(this);
  ui->okkBtn->setEnabled(false);
}

pluginAddDialog::~pluginAddDialog()
{
  delete ui;
}

void pluginAddDialog::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);
  switch (e->type()) {
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
    break;
    default:
    break;
  }
}

void pluginAddDialog::on_PluginName_textEdited(const QString &arg1)
{
  if(arg1.isEmpty())
  {
    ui->okkBtn->setEnabled(false);
  }
  else
  {
    ui->okkBtn->setEnabled(true);
  }
}



void pluginAddDialog::on_browseBtn_clicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, "Select plugin file",
                                          "./plugins/", "*.dll");
  QFileInfo info;
  info.setFile(fileName);
  ui->pluginFileEdit->setText("./plugins/" + info.fileName());
}

void pluginAddDialog::on_okkBtn_clicked()
{
  if(ui->pluginFileEdit->text().isEmpty()||
     ui->PluginName->text().isEmpty())
  {
    //...
  }
  else
  {
    QFileInfo info;
    info.setFile(ui->pluginFileEdit->text());

    emit okkPressed(info.fileName(), ui->PluginName->text());
    ui->okkBtn->setEnabled(false);
    this->close();
  }
}
