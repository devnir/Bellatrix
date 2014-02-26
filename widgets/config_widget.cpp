#include "config_widget.h"
#include "ui_config_widget.h"
#include "main_settings.h"
#include "mainwindow.h"
#include "QFile"
#include "QDir"


ConfigWidget::ConfigWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ConfigWidget)
{
  ui->setupUi(this);
  set.load();
}

ConfigWidget::~ConfigWidget()
{
  delete ui;
}

void ConfigWidget::changeEvent(QEvent *e)
{
  QWidget::changeEvent(e);
  switch (e->type()) {
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
    break;
    default:
    break;
  }
}

void ConfigWidget::updateData()
{
  if(set.view.QAtoolbarEn)
  {
    ui->QABarEnable->setChecked(true);
  }
  else
  {
    ui->QABarEnable->setChecked(false);
  }


  QDir dir("./template/");
  dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
  QStringList list = dir.entryList();
  ui->styleBox->blockSignals(true);
  ui->styleBox->clear();
  int index = 0;
  for(int i = 0; i < list.count(); i++)
  {
    ui->styleBox->addItem(list.at(i));
    if(list.at(i) == set.view.styleName)
      index = i;
  }
  ui->styleBox->blockSignals(false);
  ui->styleBox->setCurrentIndex(index);
}

void ConfigWidget::showEvent(QShowEvent *)
{
  set.load();
  updateData();
}

void ConfigWidget::on_defButton_clicked()
{
  set.toDef();
  updateData();
}

void ConfigWidget::on_QABarEnable_stateChanged(int arg1)
{
  if(arg1 == Qt::Checked)
  {
    set.view.QAtoolbarEn = 1;
    emit signalEnabledQA(true);
  }
  else if(arg1 == Qt::Unchecked)
  {
    set.view.QAtoolbarEn = 0;
    emit signalEnabledQA(false);
  }
}

void ConfigWidget::closeEvent(QCloseEvent *)
{
  set.save();
}

void ConfigWidget::on_styleBox_currentIndexChanged(int index)
{
  set.view.styleName = ui->styleBox->currentText();
  set.save();
  emit signalStyleChaged();
}
