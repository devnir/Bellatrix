#include "mainwindow.h"
#include "ui_mainwindow.h"
INT8U   OutBuff[1024];

INT32U  baudTable[_BAUD_TABLE_SIZE_] =
{
  9600,
  38400,
  115200,
  230400
};

void MainWindow::searchInit()
{
  searchPort = new QSerialPort();
  port = new QSerialPort();

  searchTimer = new QTimer();
  searchTimer->setInterval(200);
  searchTimer->setSingleShot(true);
  connect(searchPort, SIGNAL(readyRead()), this, SLOT(slotSearchDataRead()));
  connect(searchTimer, SIGNAL(timeout()), this, SLOT(slotSearchTimeout()));
  connect(port, SIGNAL(readyRead()), this, SLOT(slotSerialRead()));
}

void MainWindow::searchStart()
{
  searchIndex = 0;
  setStatusText(_LOG_NORM_, tr("Search"));
  setPortText(baudTable[searchIndex]);
  int l = Binr2ReqEncode(OutBuff, 0xC0, Binr2Single, 1);
  searchPort->setPortName(portBox->currentText());
  if(searchPort->open(QIODevice::ReadWrite))
  {
    searchPort->setFlowControl(QSerialPort::NoFlowControl);
    searchPort->setParity(QSerialPort::NoParity);
    searchPort->setDataBits(QSerialPort::Data8);
    searchPort->setBaudRate(baudTable[searchIndex], QSerialPort::AllDirections);
    searchPort->write((char *)OutBuff, l);
    searchTimer->start();
  }
  else
  {
    setStatusText(_LOG_CRIT_, tr("Open port err."));
  }
}

void MainWindow::searchStop()
{
  searchPort->close();
  ui->serialBar->setEnabled(true);
  ui->actionConnect->setEnabled(true);
  ui->actionSearch->setEnabled(true);
}

void MainWindow::searchDone()
{
  searchTimer->stop();
  MainSettings set;
  set.load();
  set.serial.portName = portBox->currentText();
  set.serial.baud = baudTable[searchIndex];
  baudBox->setCurrentIndex(searchIndex);
  setStatusText(_LOG_GOOD_, tr("Sync ok"));
  searchStop();
}

void MainWindow::slotSearchDataRead()
{
  QByteArray data = searchPort->readAll();
  INT8U ret;
  for(int i = 0; i< data.length(); i++)
  {
    ret = Binr2Unpack(data.at(i) & 0xFF);
    if(ret == 0x80)
    {
      searchDone();
    }
  }

}

void MainWindow::slotSearchTimeout()
{
  searchPort->close();
  searchIndex++;
  if(searchIndex >= _BAUD_TABLE_SIZE_)
  {
    setStatusText(_LOG_CRIT_, tr("Sync error"));
    searchStop();
    return;
  }
  setPortText(baudTable[searchIndex]);
  int l = Binr2ReqEncode(OutBuff, 0xC0, Binr2Single, 1);
  searchPort->setPortName(portBox->currentText());
  if(searchPort->open(QIODevice::ReadWrite))
  {
    searchPort->setFlowControl(QSerialPort::NoFlowControl);
    searchPort->setParity(QSerialPort::NoParity);
    searchPort->setDataBits(QSerialPort::Data8);
    searchPort->setBaudRate(baudTable[searchIndex], QSerialPort::AllDirections);
    searchPort->write((char *)OutBuff, l);
    searchTimer->start();
  }
  else
  {
    setStatusText(_LOG_CRIT_, tr("Open port err."));
  }
}

void MainWindow::avalPorts()
{
  //определение портов в системе.
    MainSettings set;
    set.load();
    QStringList pName;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
      pName << info.portName();
    }

    pName.sort();
    int index = 0;
    portBox->blockSignals(true);
    portBox->clear();
    for(int i = 0; i < pName.count(); i++)
    {
      portBox->addItem(pName.at(i));
      if(set.serial.portName == pName.at(i))
        index = i;
    }
    portBox->setCurrentIndex(index);
    portBox->blockSignals(false);

   if(pName.count() > 0)
   {
     ui->actionConnect->setEnabled(true);
     ui->actionConnect->setVisible(true);
     ui->actionSearch->setEnabled(true);
   }
}

void MainWindow::slotChangeSerialSettings()
{
  MainSettings set;
  set.load();
  set.serial.portName = portBox->currentText();
  set.serial.baud = baudTable[baudBox->currentIndex()];
  set.save();
  setPortText(baudTable[baudBox->currentIndex()]);
}

void MainWindow::slotSerialRead()
{
  QByteArray data = port->readAll();
  INT8U ret;
  for(int i = 0; i< data.length(); i++)
  {
    ret = Binr2Unpack(data.at(i) & 0xFF);
    if(ret != 0)
    {
      updateLocalData();
      updatePluginData();
    }
  }
}

void MainWindow::on_actionConnect_triggered()
{
  int l = Binr2ReqEncode(OutBuff, 0xC0, Binr2Single, 1);
  l += Binr2ReqEncode(OutBuff + l, 0x90, Binr2PVT, 1);
  ui->actionConnect->setVisible(false);
  ui->actionConnect->setEnabled(false);
  ui->actionDisconnect->setVisible(true);
  ui->actionDisconnect->setEnabled(true);
  ui->actionSearch->setEnabled(false);
  ui->serialBar->setEnabled(false);
  ui->menuPlugins->setEnabled(true);

  port->setPortName(portBox->currentText());
  if(port->open(QIODevice::ReadWrite))
  {
    port->setFlowControl(QSerialPort::NoFlowControl);
    port->setParity(QSerialPort::NoParity);
    port->setDataBits(QSerialPort::Data8);
    port->setBaudRate(baudTable[baudBox->currentIndex()], QSerialPort::AllDirections);
    port->write((char *)OutBuff, l);
    setStatusText(_LOG_GOOD_, tr("Connected"));

  }
  else
  {
    ui->actionConnect->setVisible(true);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setVisible(false);
    ui->actionDisconnect->setEnabled(false);
    ui->actionSearch->setEnabled(true);
    ui->serialBar->setEnabled(true);
    ui->menuPlugins->setEnabled(false);
    setStatusText(_LOG_CRIT_, tr("Open port err."));
  }
}

void MainWindow::on_actionDisconnect_triggered()
{
    port->close();
    ui->actionConnect->setVisible(true);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setVisible(false);
    ui->actionDisconnect->setEnabled(false);
    ui->actionSearch->setEnabled(true);
    ui->serialBar->setEnabled(true);
    ui->menuPlugins->setEnabled(false);
    setStatusText(_LOG_NORM_, tr("Disconnected"));
}
