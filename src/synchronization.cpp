#include "synchronization.h"
#include <QDebug>

INT32U baudTable[_BAUD_TABLE_SIZE_] =
{
  9600,
  38400,
  115200,
  230400,
};

BINR2_POINTERS b2Ptr;
BINR2x80 x80;
INT8U outBuff[128];

Synchronization::Synchronization()
{
  timer = new QTimer();
  timer->setSingleShot(false);
  timer->setInterval(1000);
  port = new QSerialPort();
  curentIndex = 0;
  b2Ptr.p80 = &x80;

  connect(port, SIGNAL(readyRead()), this, SLOT(slotDataRead()));
  connect(timer, SIGNAL(timeout()), this, SLOT(slotTimeout()));

}

void Synchronization::start()
{
  curentIndex = 0;
  Binr2Init(b2Ptr);
  set.load();
  int l = Binr2ReqEncode(outBuff, 0xC0, Binr2Single, 1);
  port->setPortName(set.serial.portName);
  set.serial.baud = baudTable[curentIndex];
  set.save();
  if(port->open(QIODevice::ReadWrite))
  {
    port->setFlowControl(QSerialPort::NoFlowControl);
    port->setParity(QSerialPort::NoParity);
    port->setDataBits(QSerialPort::Data8);
    if(!port->setBaudRate(baudTable[curentIndex], QSerialPort::AllDirections))
      qDebug()  << port->error();
    port->write((char *)outBuff, l);
    timer->start();
  }
  else
  {
    //emit ended(0);
  }
}

void Synchronization::stop()
{
  timer->stop();
  port->close();
}

void Synchronization::searchDone()
{
  timer->stop();
  port->close();
  //emit this->ended(1);
}

void Synchronization::slotTimeout()
{
  port->close();
  curentIndex++;
  if(curentIndex > _BAUD_TABLE_SIZE_)
  {
    //emit this->ended(0);
    return;
  }

  int l = Binr2ReqEncode(outBuff, 0xC0, Binr2Single, 1);
  port->setPortName(set.serial.portName);
  set.serial.baud = baudTable[curentIndex];
  set.save();
  if(port->open(QIODevice::ReadWrite))
  {
    port->setFlowControl(QSerialPort::NoFlowControl);
    port->setParity(QSerialPort::NoParity);
    port->setDataBits(QSerialPort::Data8);
    if(!port->setBaudRate(baudTable[curentIndex], QSerialPort::AllDirections))
      qDebug()  << port->error();
    port->write((char *)outBuff, l);
    timer->start();
  }
  else
  {
    //emit ended(0);
  }
}

void Synchronization::slotDataRead()
{
  QByteArray data = port->readAll();
  INT8U ret;
  for(int i = 0; i< data.length(); i++)
  {
    ret = Binr2Unpack(data.at(i) & 0xFF);
    //qDebug("0x%02X", data.at(i) & 0xFF);
    if(ret == 0x80)
    {
      searchDone();
    }
  }
}
