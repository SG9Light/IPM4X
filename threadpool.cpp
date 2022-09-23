#include "threadpool.h"
#include "consts.h"
#include "qdatetime.h"
#include <QElapsedTimer>
#include <QThread>
#include <QDebug>

Task::Task(QObject *parent) : QObject(parent), QRunnable()
{
    //设置自动析构
    setAutoDelete(false);

}
/*获取串口数据*/
void Task::receData(QByteArray byte, int type)
{
    rxmsg.messagetype = type;
    rxmsg.len = byte.size();
    memcpy(rxmsg.data, byte.data(), byte.size());
    emit u8_rxdata(byte);
    byte.clear();
}

/************************************************************************************
函数名称:	rxmsg_analyzeapp
功能描述:	应用层数据解析
参数：串口接收buff
***********************************************************************************/
void Task::rxmsg_analyzeapp(_PORT_RXMSG *psdata)
{
    uint16_t crc_cal,crc_mem,calcrc_len;

    //校验码计算
    calcrc_len = psdata->len - 2;
    crc_cal = modbuscrc->crc16_modbus(psdata->data,calcrc_len);

    crc_mem = psdata->data[calcrc_len]<< 8;  //低位
    crc_mem |= psdata->data[calcrc_len+1];  //高位

    if(crc_cal != crc_mem){
        qDebug()<<"数据解析校验测错误";
        return;
    }
    upsmodeordinal = psdata->data[0];

    switch (psdata->data[1]) {
    case FUNTION_QUERY_CODE:
        mod_03h(psdata);
        break;

    case FUNTION_CONTROL_CODE:
        break;
    default:
        break;
    }
}

/************************************************************************************
函数名称:	mod_03h
功能描述:	03h数据解析
参数：串口接收buff
***********************************************************************************/
void Task::mod_03h(_PORT_RXMSG *psdata)
{
    QDateTime *updata_time = new QDateTime;
    updatatime[psdata->data[0]] = updata_time->currentDateTime().toString("hh:mm:ss");

    int j = 3;
    uint16_t *ups_data;

    if((int)psdata->messagetype == Alarm){
 //       qDebug()<<"******************************告警量******************************";
        ups_data = (uint16_t *)&m_ups_waring[psdata->data[0]];
        for (int i = 0; i < ALARM_LEN; ++i) {
            ups_data[i] = psdata->data[j] << 8 | psdata->data[j+1];
            j = j+2;
        }

    }else if(psdata->messagetype == State){
//        qDebug()<<"******************************状态******************************";
        ups_data = (uint16_t *)&m_ups_state[psdata->data[0]];
        for (int i = 0; i < STATE_LEN; ++i) {
            ups_data[i] = psdata->data[j] << 8 | psdata->data[j+1];
            j = j+2;
        }

    }else if(psdata->messagetype == Analog){
//        qDebug()<<"******************************模拟量******************************";
        ups_data = (uint16_t *)&m_ups_analog[psdata->data[0]];
        for (int i = 0; i < ANALOG_LEN; ++i) {
            ups_data[i] = psdata->data[j] << 8 | psdata->data[j+1];
            j = j+2;
        }
    }else{
//        qDebug()<<"******************************超出范围******************************";
    }
    ups_data = nullptr;
    delete updata_time;
}


void Task::run(){
    QElapsedTimer time;
    time.start();
    QThread::msleep(1); // 处理任务
    rxmsg_analyzeapp(&rxmsg);
 //   qDebug() <<"子线程任务处理完成，共耗时： "<<time.elapsed() <<" ms";
    emit taskflash(true);

}

