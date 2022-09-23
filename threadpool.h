#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "modbusslave.h"
#include "types.h"
#include <QObject>
#include <QRunnable>

class Task:public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit Task(QObject *parent = nullptr);

public:
    void receData(QByteArray byte, int type);

private:
    void rxmsg_analyzeapp(_PORT_RXMSG *psdata);

    void mod_03h(_PORT_RXMSG *psdata);

signals:
    bool taskflash(bool ok);

    void u8_rxdata(QByteArray rdata);

protected:
    void run() override;

private:
    modbusslave *modbuscrc;

    _PORT_RXMSG rxmsg;
};

#endif // THREADPOOL_H
