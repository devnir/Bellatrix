#include "plugineditdialog.h"
#include "ui_plugineditdialog.h"

pluginEditDialog::pluginEditDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::pluginEditDialog)
{
  ui->setupUi(this);
}

pluginEditDialog::~pluginEditDialog()
{
  delete ui;
}

void pluginEditDialog::changeEvent(QEvent *e)
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
