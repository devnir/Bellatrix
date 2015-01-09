#include "mainwindow.h"
#include "ui_mainwindow.h"
INT8U   OutBuff[1024];
INT8U   nmeaToB2[128];
INT32U  baudTable[_BAUD_TABLE_SIZE_] =
{
  9600,
  38400,
  115200,
  230400
};

void MainWindow::searchInit()
{
  searchPort = new QSerialPort(this);
  port = new QSerialPort(this);

  searchTimer = new QTimer();
  searchTimer->setInterval(300);
  searchTimer->setSingleShot(true);


  connect(port, SIGNAL(readyRead()), this, SLOT(slotSerialRead()));
  connect(searchPort, SIGNAL(readyRead()), this, SLOT(slotSearchDataRead()));
  connect(searchTimer, SIGNAL(timeout()), this, SLOT(slotSearchTimeout()));
}

void MainWindow::searchStart()
{
  searchIndex = 0;
  setStatusText(_LOG_NORM_, tr("Search"));
  setPortText(baudTable[searchIndex]);
  int l = Binr2ReqEncode(OutBuff, 0x90, Binr2PVT, 1);
  searchPort->setPortName(portBox->currentText());
  if(searchPort->open(QIODevice::ReadWrite))
  {
    connect(searchPort, SIGNAL(readyRead()), this, SLOT(slotSearchDataRead()));
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
  disconnect(searchPort, SIGNAL(readyRead()), this, SLOT(slotSearchDataRead()));
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
  //searchTimer->stop();
  QByteArray data = searchPort->readAll();
  INT8U ret;
  for(int i = 0; i< data.length(); i++)
  {
    ret = Binr2Unpack(data.at(i) & 0xFF);
    if(ret == 0x80)
    {
      searchDone();
      disconnect(searchPort, SIGNAL(readyRead()), this, SLOT(slotSearchDataRead()));
      return;
    }
  }
  //searchTimer->start();
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
  int nmeaL = 0;
  l += store8lu(OutBuff + l, 0x02);
  l += store8lu(OutBuff + l, 0x00);
  l += store8lu(OutBuff + l, 0x00);
  l += store32lu(OutBuff + l, baudTable[baudBox->currentIndex()]);
  l = Binr2Encode(OutBuff, OutBuff, l);
  nmeaL = sprintf((char *)nmeaToB2, "$PORZA,0,%d,4", baudTable[baudBox->currentIndex()]);
  INT8U Sum = 0;

  for(INT32U i = 1; i < nmeaL; i++)
    Sum ^= nmeaToB2[i];
  nmeaL += sprintf((char *)nmeaToB2 + nmeaL, "*%02x\r\n", Sum);


  searchPort->setPortName(portBox->currentText());
  if(searchPort->open(QIODevice::ReadWrite))
  {
    connect(searchPort, SIGNAL(readyRead()), this, SLOT(slotSearchDataRead()));
    searchPort->setFlowControl(QSerialPort::NoFlowControl);
    searchPort->setParity(QSerialPort::NoParity);
    searchPort->setDataBits(QSerialPort::Data8);
    searchPort->setBaudRate(baudTable[searchIndex], QSerialPort::AllDirections);
    searchPort->write((char *)OutBuff, l);
    searchPort->write((char *)nmeaToB2, nmeaL);
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
  if(ui->recordToolButton->isChecked())
  {
    logFile.open(QIODevice::Append);
    logFile.write(data);
    logFileSize += data.length();
    ui->fileSize->setText(fileSizeToStr(logFileSize));
    logFile.close();
  }

  for(int i = 0; i< data.length(); i++)
  {
    ret = Binr2Unpack(data.at(i) & 0xFF);
    if(ret != 0)
    {
//      qDebug("Income 0x%02X, Size: %d", ret, Binr2DataSize);
/*      if(ret == 0x80)
      {
        qDebug("0x80 0x%02X 0x%02X", Binr2DataBuff[1], Binr2DataBuff[2] );
      }
*/
      updateLocalData();
      updatePluginData();
    }
  }
}

void MainWindow::on_actionConnect_triggered()
{
  int l = Binr2ReqEncode(OutBuff, 0xC0, Binr2Single, 1);
  l += Binr2ReqEncode(OutBuff + l, 0x90, Binr2PVT, 1);
  l += Binr2ReqEncode(OutBuff + l, 0x92, Binr2Meas, 1);
  ui->actionConnect->setVisible(false);
  ui->actionConnect->setEnabled(false);
  ui->actionDisconnect->setVisible(true);
  ui->actionDisconnect->setEnabled(true);
  ui->actionSearch->setEnabled(false);
  ui->serialBar->setEnabled(false);

  port->setPortName(portBox->currentText());
  if(port->open(QIODevice::ReadWrite))
  {
    port->setFlowControl(QSerialPort::NoFlowControl);
    port->setParity(QSerialPort::NoParity);
    port->setDataBits(QSerialPort::Data8);
    port->setBaudRate(baudTable[baudBox->currentIndex()], QSerialPort::AllDirections);
    setPortText(baudTable[baudBox->currentIndex()]);
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
    int l = Binr2ReqEncode(OutBuff, 0x00, Binr2CancelAll, 0);
    port->write((char *)OutBuff, l);
    port->waitForBytesWritten(100);
    port->close();
    ui->actionConnect->setVisible(true);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setVisible(false);
    ui->actionDisconnect->setEnabled(false);
    ui->actionSearch->setEnabled(true);
    ui->serialBar->setEnabled(true);
    setStatusText(_LOG_NORM_, tr("Disconnected"));
}
