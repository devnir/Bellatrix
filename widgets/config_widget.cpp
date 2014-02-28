#include "config_widget.h"
#include "ui_config_widget.h"
#include "main_settings.h"
#include "mainwindow.h"
#include "QFile"
#include "QDir"

QStringList pluginList[_MAX_PLUGINS_];

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

  if(set.view.QAseriaEn)
    ui->QASBarEn->setChecked(true);
  else
    ui->QASBarEn->setChecked(false);


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

void ConfigWidget::on_styleBox_currentIndexChanged(int)
{
  set.view.styleName = ui->styleBox->currentText();
  set.save();
  emit signalStyleChaged();
}

void ConfigWidget::on_QASBarEn_stateChanged(int arg1)
{
  if(arg1 == Qt::Checked)
  {
    set.view.QAseriaEn = 1;
    emit signalEnabledQAS(true);
  }
  else if(arg1 == Qt::Unchecked)
  {
    set.view.QAseriaEn = 0;
    emit signalEnabledQAS(false);
  }
}


void ConfigWidget::on_tabWidget_currentChanged(int index)
{
  if(index == 1)
  {
    ui->pluginWiew->clear();
    int row = 1;
    ui->pluginWiew->insertRow(6);
    for(int i = 0; i < _MAX_PLUGINS_; i++)
    {
      if(plugins[i].validity == 1)
      {
        ui->pluginWiew->insertRow(0);
        QTableWidgetItem *fName = new QTableWidgetItem;
        fName->setText(plugins[i].fileName);
        QTableWidgetItem *fDesc = new QTableWidgetItem;
        fDesc->setText(plugins[i].name);
        ui->pluginWiew->setItem(0, 0, fName);
        ui->pluginWiew->setItem(0, 1, fDesc);
        row++;
      }
    }
  }
}
