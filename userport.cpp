#include "userport.h"
#include "consts.h"

userport::userport(QObject *parent)
    : QObject{parent}
{

    connect(mTxQTimer, &QTimer::timeout, this, &userport::timer1ms);
    connect(mSerial, &QSerialPort::readyRead, this,&userport::recevice);
    connect(mRxQTimer, &QTimer::timeout, this, &userport::rxend);

    this->moveToThread(portThread);
    mSerial->moveToThread(portThread);
    mTxQTimer->moveToThread(portThread);
    mRxQTimer->moveToThread(portThread);
    portThread->start();
}

userport::~userport()
{
    mTxQTimer->deleteLater();
    mRxQTimer->deleteLater();

    mSerial->close();
    mSerial->deleteLater();

    portThread->quit();
    portThread->wait();
    portThread->deleteLater();

}
/*打开串口*/
void userport::openport()
{
    if(mSerial->isOpen()){
        mSerial->close();
        mTxQTimer->stop();
        emit openstate(2);
    }
    else{
        mSerial->setPortName(portname);
        mSerial->setBaudRate(QSerialPort::Baud9600);
        mSerial->setDataBits(QSerialPort::Data8);
        mSerial->setParity(QSerialPort::NoParity);
        mSerial->setStopBits(QSerialPort::OneStop);
        if(mSerial->open(QSerialPort::ReadWrite) == true){
            mTxQTimer->start(200);
            emit openstate(1);
        }else{
            emit openstate(3);
        }
    }
}
/*数据接收*/
void userport::recevice()
{
    //9600/8/1000=1.2Byte/ms >>> time = MAX(42)/1.2 = 35ms---->50MAX(60Byte)
    mRxQTimer->start(100);
    rxmsg = rxmsg.append(mSerial->readAll());
}
/*数据发送*/
void userport::send(uint8_t *_pBuf, uint16_t _usLen)
{
    QByteArray arr = QByteArray::fromRawData((char *)_pBuf,_usLen);
    uint16_t crc16 = modbuscrc->crc16_modbus(_pBuf,_usLen);

    arr = arr.append(crc16>>8).append(crc16);
    mSerial->write(arr);
    emit txdata(arr);
}
/*查询当前可用串口*/
void userport::rportname(QString str)
{
    portname = str;
}

void userport::rxend()
{
    mRxQTimer->stop();
    if(!rxmsg.isEmpty())emit rxdata(rxmsg, Querydatatype);
    rxmsg.clear();
}

/*1s定时查询*/
void userport::timer1ms()
{
    uint8_t sendbuf[6] = {0};
    sendtype++;
    if(sendtype > CMD_PRD_TABL_SIZE){
        sendtype = 1;
        iDestination++;
        if(iDestination >= MODE_NUM){
            iDestination = 1;
        }
    }

    sendbuf[DESTINATION] = iDestination;
    sendbuf[FUNCODE] = FUNTION_QUERY_CODE;
    sendbuf[DATA_HIGHT] = 0;
    switch (sendtype) {
    case 1:
        memset(&m_ups_waring[iDestination], 0, sizeof(m_ups_waring[iDestination]));
        sendbuf[ADDR_HIGHT] = WARN1_REG_ADDR>>8;
        sendbuf[ADDR_LOW] = (uint8_t)WARN1_REG_ADDR;
        sendbuf[DATA_LOW] = ALARM_LEN;
        Querydatatype = Alarm;
        break;

    case 2:
        memset(&m_ups_state[iDestination], 0, sizeof(m_ups_state[iDestination]));
        sendbuf[ADDR_HIGHT] = STATUS1_REG_ADDR>>8;
        sendbuf[ADDR_LOW] = (uint8_t)STATUS1_REG_ADDR;
        sendbuf[DATA_LOW] = STATE_LEN;
        Querydatatype = State;
        break;

    case 3:
        memset(&m_ups_analog[iDestination], 0, sizeof(m_ups_analog[iDestination]));
        sendbuf[ADDR_HIGHT] = IN_VOLT_A_REG_ADDR>>8;
        sendbuf[ADDR_LOW] = (uint8_t)IN_VOLT_A_REG_ADDR;
        sendbuf[DATA_LOW] = ANALOG_LEN;
        Querydatatype = Analog;
        break;
    default:
        break;
    }

    send(sendbuf,6);
    emit sendnum(iDestination);
}

void userport::ac_setvcmd(int id, int addr, int data)
{
    mTxQTimer->stop();
    uint8_t buf[6] = {0};

    buf[0] = id;
    buf[1] = 0x06;
    buf[2] = addr >> 8;
    buf[3] = addr;
    buf[4] = data >> 8;
    buf[5] = data;
    send(buf,6);
    mTxQTimer->start(200);
}



