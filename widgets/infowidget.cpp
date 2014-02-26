#include "infowidget.h"
#include "ui_infowidget.h"
#include "logging.h"

InfoWidget::InfoWidget(QWidget *parent) :
  QTabWidget(parent),
  ui(new Ui::InfoWidget)
{
  ui->setupUi(this);
}

InfoWidget::~InfoWidget()
{
  delete ui;
}

void InfoWidget::changeEvent(QEvent *e)
{
  QTabWidget::changeEvent(e);
  switch (e->type()) {
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
    break;
    default:
    break;
  }
}

void InfoWidget::showEvent(QShowEvent *)
{
  ui->logEdit->clear();
  QStringList strList = getLogs();
  for(int i = 0; i< strList.count(); i++)
  {
    ui->logEdit->append(strList.at(i));
  }
}
