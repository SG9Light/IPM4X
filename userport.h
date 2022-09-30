#ifndef USERPORT_H
#define USERPORT_H

#include "acsoucedialog.h"
#include "consts.h"
#include "modbusslave.h"
#include "types.h"

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QThread>
#include <QDebug>

class userport : public QObject
{
    Q_OBJECT
public:
    explicit userport(QObject *parent = nullptr);
    ~userport();

public:
    void send(uint8_t *_pAddr, uint16_t _usLen);

    void rportname(QString str);

    void rxend();

public slots:
    void openport();

    void recevice();

    void timer1ms();

    void ac_setvcmd(int id, int addr, int data);

signals:
    void openstate(int n);

    void sendnum(int n);

    void txdata(QByteArray byte);

    void rxdata(QByteArray byte, int type);

private:
    QThread *portThread = new QThread;

    QSerialPort *mSerial = new QSerialPort;

    QTimer *mTxQTimer = new QTimer;

    QTimer *mRxQTimer = new QTimer;

    modbusslave *modbuscrc;

private:
    TTs_PrdTmrDef PrdSchdlrTbl[CMD_PRD_TABL_SIZE];

    QString portname;

    QByteArray rxmsg;

    uint8_t Querydatatype;

    uint8_t iDestination, sendtype, id, vloue;

};

#endif // USERPORT_H
