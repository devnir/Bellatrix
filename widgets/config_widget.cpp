#include "config_widget.h"
#include "ui_config_widget.h"
#include "main_settings.h"
#include "mainwindow.h"
#include "QFile"
#include "QDir"

//QStringList pluginList[_MAX_PLUGINS_];

ConfigWidget::ConfigWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ConfigWidget)
{
  ui->setupUi(this);
  set.load();
  on_tabWidget_currentChanged(1);

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
    for(int i = 0; i < ui->pluginWiew->rowCount(); i++)
      ui->pluginWiew->removeRow(i);
    int row = 1;
    for(int i = 0; i < pluginList.size(); i++)
    {
      TPlugin plugin = pluginList.at(i);
      ui->pluginWiew->insertRow(0);
      QTableWidgetItem *fName = new QTableWidgetItem;
      fName->setText(plugin.fileName);
      QTableWidgetItem *pName = new QTableWidgetItem;
      QString str;
      str.sprintf(" v%d.%d", plugin.version.major, plugin.version.minor);
      str.prepend(plugin.version.name);
      pName->setText(str);

      QTableWidgetItem *fDesc = new QTableWidgetItem;
      fDesc->setText(plugin.version.desc);

      ui->pluginWiew->setItem(0, 0, fName);
      ui->pluginWiew->setItem(0, 1, pName);
      ui->pluginWiew->setItem(0, 2, fDesc);
      row++;
    }
  }
}

