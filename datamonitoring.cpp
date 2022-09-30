#include "datamonitoring.h"
#include "types.h"
#include "ui_datamonitoring.h"

datamonitoring::datamonitoring(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::datamonitoring)
{
    ui->setupUi(this);

    disptableinit();
    modelboxinit();

    ui->mpoll_progressBar->setMaximum(MODE_NUM);
    connect(ref, &QTimer::timeout, this, &datamonitoring::upsupdata);
    ref->start(1000);
}

datamonitoring::~datamonitoring()
{
    delete ui;
}

void datamonitoring::modelboxinit()
{
    ui->datatype_comboBox->addItem("请选择");

    QStringList modelnum;
    for (int var = 0; var < MODE_NUM; ++var) {
        modelnum << tr("模块%1").arg(var + 1);
    }
    ui->modenum_comboBox->addItems(modelnum);

}

void datamonitoring::disptableinit()
{
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    settableheader();

    QString cellStyle =
            R"(
        QTableView
        {
            text-align:center;
            background-color: rgba(255, 255, 255, 0);
            alternate-background-color:#e3edf9;
            font:14px "微软雅黑";
            color:#677483;
            gridline-color: #ccddf0;
        }
    )";

    const QString horizontalHeaderStyle =
            R"(
        QHeaderView::section {
            color: black;
            font:bold 14px "微软雅黑";
            text-align:center;
            height:32px;
            background-color: #d1dff0;
            border:1px solid #8faac9;
            border-left:none;
        }
    )";

    const QString verticalHeaderStyle =
            R"(
        QHeaderView::section {
            color: black;
            font:bold 14px "微软雅黑";
            width:60px;
            text-align:center;

            background-color: #d1dff0;
            border:1px solid #8faac9;
            border-left:none;
        }
    )";

    ui->tableWidget->setStyleSheet(cellStyle);
    ui->tableWidget->horizontalHeader()->setStyleSheet(horizontalHeaderStyle);
    ui->tableWidget->verticalHeader()->setStyleSheet(verticalHeaderStyle);

}

void datamonitoring::upsupdata()
{
    if(upsmodeordinal != ui->modenum_comboBox->currentIndex()){
        upsmodeordinal = 0;
        return;
    }
    QString str = ui->datatype_comboBox->currentText();
    if( str != "单模块变量查询"){
        ui->tableWidget->setRowCount(24);
        if(distype == ALARM_TPYE){
            onealarmmonitor(str);
        }else if(distype == STATE_TPYE){
            onestatemonitor(str);
        }else if(distype == ANALOG_TPYE){
            oneanalogmonitor(str);
        }else{
            //Do Nothing
        }
    }
    else{
        if(distype == ANALOG_TPYE){
            dispanalog(ui->modenum_comboBox->currentIndex());
        }
        else if(distype == STATE_TPYE){
            dispsate(ui->modenum_comboBox->currentIndex());
        }
        else if(distype == ALARM_TPYE){
            dispalarm(ui->modenum_comboBox->currentIndex());
        }
    }
    upsmodeordinal = 0;
}

void datamonitoring::appendOneRow(int row, QString name, float value, QString unit, QString time)
{
    QTableWidgetItem *nameItem = new QTableWidgetItem(name);
    QTableWidgetItem *valueItem = new QTableWidgetItem(QString("%1").arg(value));
    QTableWidgetItem *unitItem = new QTableWidgetItem(unit);
    QTableWidgetItem *timeItem = new QTableWidgetItem(time);

    nameItem->setTextAlignment(Qt::AlignCenter);
    unitItem->setTextAlignment(Qt::AlignCenter);
    valueItem->setTextAlignment(Qt::AlignCenter);
    timeItem->setTextAlignment(Qt::AlignCenter);

    ui->tableWidget->setItem(row, 0, nameItem);
    ui->tableWidget->setItem(row, 1, valueItem);
    ui->tableWidget->setItem(row, 2, unitItem);
    ui->tableWidget->setItem(row, 3, timeItem);

}

void datamonitoring::dispalarm(int modelnum)
{
    ui->mAnalog_pushButton->setStyleSheet("");
    ui->mWaring_pushButton->setStyleSheet("background:green");
    ui->mState_pushButton->setStyleSheet("");
    ui->tableWidget->setRowCount(36);

    int tempa = 0;
    QString time = updatatime[modelnum];
    QString str;

    //***************Alarm word1******************
    tempa = m_ups_waring[modelnum].word1.bFanFaultshow;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(0, "风扇故障", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(0, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word1.bDCHighAbnormal;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(1, "母线高压异常", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(1, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word1.bInvFault;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(2, "逆变器故障", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(2, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word1.bOVTMP;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(3, "过温故障", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(3, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word1.bShortFault;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(4, "短路故障", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(4, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word1.bDischgFault;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(5, "放电器故障", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(5, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word1.bRecFault;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(6, "整流器故障", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(6, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word1.bBattStUpInverse;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(7, "电池反接", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(7, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word1.bDCLowAbnormal;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(8, "直流母线电压低", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(8, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word1.bBusOVPAbnormal;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(9, "硬件检测母线电压高", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(9, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word1.bInvOvloadTimeout;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(10, "输出过载延时到", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(10, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word1.bOvload;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(11, "输出过载告警标志", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(11, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word1.bBattLowFault;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(12, "电池电压低", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(12, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word1.bBattHighFault;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(13, "电池高压故障", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(13, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word1.bMasterCANAFault;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(14, "主机CANA故障", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(14, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word1.bCANAFault;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(15, "CANA故障", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(15, 2)->setBackground(Qt::red);

    //***************Alarm word2******************
    tempa = m_ups_waring[modelnum].word2.bMainOk;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(16, "主路正常", tempa, str, time);
    if(tempa != 1)ui->tableWidget->item(16, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word2.bVin;
    if(tempa == 0)str = "正常";
    else if(tempa == 1)str = "低异常";
    else if(tempa == 2)str = "欠压";
    else if(tempa == 4)str = "过压";
    else if(tempa == 8)str = "高异常";
    else str = "数据错误";
    appendOneRow(17, "交流电压", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(17, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word2.bFin;
    if(tempa == 0)str = "正常";
    else if(tempa == 1)str = "低异常";
    else if(tempa == 2)str = "高异常";
    else str = "数据错误";
    appendOneRow(18, "交流频率", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(18, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word2.bVbp;
    if(tempa == 0)str = "正常";
    else if(tempa == 1)str = "低异常";
    else if(tempa == 2)str = "高异常";
    else str = "数据错误";
    appendOneRow(19, "旁路电压", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(19, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word2.bFbp;
    if(tempa == 0)str = "正常";
    else if(tempa == 1)str = "低异常";
    else if(tempa == 2)str = "高异常";
    else str = "数据错误";
    appendOneRow(20, "旁路频率", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(20, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word2.bParaAddOverlap;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(21, "并机地址冲突", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(21, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word2.bSwitchTimesClamp;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(22, "切换次数到", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(22, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word2.bParaCurShareFault;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(23, "均流故障", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(23, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word2.bDischgFaultSoftSt;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(24, "放电器软起故障", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(24, 2)->setBackground(Qt::red);

    //***************Alarm word3******************
    tempa = m_ups_waring[modelnum].word3.bDischgFaultDCbusHigh;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(25, "放电器直流母线高", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(25, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word3.bDischgFaultDCbusLow;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(26, "放电器直流母线低", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(26, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word3.bDischgFaultDCbusShort;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(27, "放电器直流母线短路", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(27, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word3.bRecFaultHigh;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(28, "整流直流母线高", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(28, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word3.bRecFaultLow;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(29, "整流直流母线低", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(29, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word3.bPassiveSSDCAbnormal;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(30, "被动软起失败", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(30, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word3.bActiveSSDCAbnormal;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(31, "主动软起失败", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(31, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word3.bAuxPowerFault;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(32, "辅助电源故障", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(32, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word3.bCarrierSyncFault;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(33, "高频同步信号故障", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(33, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word3.bInvLowSyncFault;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(34, "工频同步信号故障", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(34, 2)->setBackground(Qt::red);

    tempa = m_ups_waring[modelnum].word3.bPhaseOutFault;
    if(tempa == 0)str = "否";
    else if(tempa == 1)str = "是";
    else str = "数据错误";
    appendOneRow(35, "单三相故障", tempa, str, time);
    if(tempa != 0)ui->tableWidget->item(35, 2)->setBackground(Qt::red);

}

void datamonitoring::dispsate(int modelnum)
{    
    ui->mAnalog_pushButton->setStyleSheet("");
    ui->mWaring_pushButton->setStyleSheet("");
    ui->mState_pushButton->setStyleSheet("background:green");
    ui->tableWidget->setRowCount(30);

    int tempa = 0;
    QString time = updatatime[modelnum];
    QString state;

    //***************State word1******************
    tempa = m_ups_state[modelnum].word1.bDcBusPreSoftStIng;
    if(tempa == 0)state = "断开";
    else if(tempa == 1)state = "闭合";
    else state = "数据错误";
    appendOneRow(0, "维修空开状态", tempa, state, time);

    tempa = m_ups_state[modelnum].word1.bDcBusSoftStOver;
    if(tempa == 0)state = "否";
    else if(tempa == 1)state = "是";
    else state = "数据错误";
    appendOneRow(1, "母线软起成功", tempa, state, time);

    tempa = m_ups_state[modelnum].word1.bDischg;
    if(tempa == 0)state = "关闭";
    else if(tempa == 1)state = "被动软启动";
    else if(tempa == 2)state = "主动软启动";
    else if(tempa == 3)state = "启动完毕";
    else state = "数据错误";
    appendOneRow(2, "放电器状态", tempa, state, time);

    tempa = m_ups_state[modelnum].word1.bPFC;
    if(tempa == 0)state = "关闭";
    else if(tempa == 1)state = "软启动";
    else if(tempa == 3)state = "启动完毕";
    else state = "数据错误";
    appendOneRow(3, "PFC状态", tempa, state, time);

    tempa = m_ups_state[modelnum].word1.bInv;
    if(tempa == 0)state = "关闭";
    else if(tempa == 1)state = "软启动";
    else if(tempa == 3)state = "启动完毕";
    else state = "数据错误";
    appendOneRow(4, "逆变器状态", tempa, state, time);

    tempa = m_ups_state[modelnum].word1.bDcBusPreSoftStIng;
    if(tempa == 0)state = "否";
    else if(tempa == 1)state = "是";
    else state = "数据错误";
    appendOneRow(5, "主动软起中", tempa, state, time);

    tempa = m_ups_state[modelnum].word1.bDcBusPreSoftStOver;
    if(tempa == 0)state = "否";
    else if(tempa == 1)state = "是";
    else state = "数据错误";
    appendOneRow(6, "主动软启动完成", tempa, state, time);

    tempa = m_ups_state[modelnum].word1.bSupply;
    if(tempa == 0)state = "均不供电";
    else if(tempa == 1)state = "旁路供电";
    else if(tempa == 2)state = "逆变供电";
    else state = "数据错误";
    appendOneRow(7, "供电状态", tempa, state, time);

    tempa = m_ups_state[modelnum].word1.bSnglPhsIn;
    if(tempa == 0)state = "三相输入";
    else if(tempa == 1)state = "单相输入";
    else state = "数据错误";
    appendOneRow(8, "输入相序", tempa, state, time);

    tempa = m_ups_state[modelnum].word1.bSilentBeep;
    if(tempa == 0)state = "否";
    else if(tempa == 1)state = "是";
    else state = "数据错误";
    appendOneRow(9, "消音状态", tempa, state, time);

    //***************State word2******************
    tempa = m_ups_state[modelnum].word2.bPhaseTrace;
    if(tempa == 0)state = "本振";
    else if(tempa == 1)state = "跟旁路";
    else if(tempa == 2)state = "跟输出（预留）";
    else if(tempa == 3)state = "跟LBS源（预留）";
    else state = "数据错误";
    appendOneRow(10, "相位跟踪标志", tempa, state, time);

    tempa = m_ups_state[modelnum].word2.bDischOnEnable;
    if(tempa == 0)state = "不允许";
    else if(tempa == 1)state = "允许";
    else state = "数据错误";
    appendOneRow(11, "放电器开机允许状态", tempa, state, time);

    tempa = m_ups_state[modelnum].word2.bRecOnEnable;
    if(tempa == 0)state = "不允许";
    else if(tempa == 1)state = "允许";
    else state = "数据错误";
    appendOneRow(12, "整流器开机允许状态", tempa, state, time);

    tempa = m_ups_state[modelnum].word2.bBatLevel;
    if(tempa == 0)state = "110V";
    else if(tempa == 1)state = "220V";
    else if(tempa == 2)state = "750V";
    else state = "数据错误";
    appendOneRow(13, "直流电压等级", tempa, state, time);

    tempa = m_ups_state[modelnum].word2.bAgingMode;
    if(tempa == 0)state = "正常模式";
    else if(tempa == 1)state = "自老化模式";
    else state = "数据错误";
    appendOneRow(14, "自老化模式", tempa, state, time);

    tempa = m_ups_state[modelnum].word2.bOutPhaMode;
    if(tempa == 0)state = "单相";
    else if(tempa == 1)state = "三相";
    else state = "数据错误";
    appendOneRow(15, "输出单三相", tempa, state, time);

    tempa = m_ups_state[modelnum].word2.bOutPha;
    if(tempa == 0)state = "单相A相";
    else if(tempa == 1)state = "A相";
    else if(tempa == 2)state = "B相";
    else if(tempa == 3)state = "C相";
    else state = "数据错误";
    appendOneRow(16, "输出相位信息", tempa, state, time);

    tempa = m_ups_state[modelnum].word2.bBpSupplyPermit;
    if(tempa == 0)state = "不允许";
    else if(tempa == 1)state = "允许";
    else state = "数据错误";
    appendOneRow(17, "旁路供电允许", tempa, state, time);

    tempa = m_ups_state[modelnum].word2.bOutFinAdaptive;
    if(tempa == 0)state = "不自适应";
    else if(tempa == 1)state = "自适应";
    else state = "数据错误";
    appendOneRow(18, "输出频率自适应", tempa, state, time);

    tempa = m_ups_state[modelnum].word2.bOutVoltLevel;
    if(tempa == 0)state = "220V";
    else if(tempa == 1)state = "230V";
    else if(tempa == 2)state = "240V";
    else state = "数据错误";
    appendOneRow(19, "输出电压等级", tempa, state, time);

    tempa = m_ups_state[modelnum].word2.bOutFinLevel;
    if(tempa == 0)state = "50Hz";
    else if(tempa == 1)state = "60Hz";
    else state = "数据错误";
    appendOneRow(20, "输出频率等级", tempa, state, time);

    //***************State word3******************
    tempa = m_ups_state[modelnum].word3.bBpFinRang;
    if(tempa == 0)state = "±1Hz";
    else if(tempa == 1)state = "±2Hz";
    else if(tempa == 2)state = "±3Hz";
    else state = "数据错误";
    appendOneRow(21, "旁路频率范围", tempa, state, time);

    tempa = m_ups_state[modelnum].word3.bBpVoltRang;
    if(tempa == 0)state = "±10%";
    else if(tempa == 1)state = "±15%";
    else state = "数据错误";
    appendOneRow(22, "旁路电压范围", tempa, state, time);

    tempa = m_ups_state[modelnum].word3.bOutPower;
    if(tempa == 0)state = "3KWA";
    else if(tempa == 1)state = "5KWA";
    else state = "数据错误";
    appendOneRow(23, "输出功率状态", tempa, state, time);

    tempa = m_ups_state[modelnum].word3.bSleepMode;
    if(tempa == 0)state = "否";
    else if(tempa == 1)state = "是";
    else state = "数据错误";
    appendOneRow(24, "休眠模式", tempa, state, time);

    tempa = m_ups_state[modelnum].word3.bSelfTest;
    if(tempa == 0)state = "否";
    else if(tempa == 1)state = "是";
    else state = "数据错误";
    appendOneRow(25, "模块自检", tempa, state, time);

    tempa = m_ups_state[modelnum].word3.bBpMode;
    if(tempa == 0)state = "否";
    else if(tempa == 1)state = "是";
    else state = "数据错误";
    appendOneRow(26, "旁路模式", tempa, state, time);

    tempa = m_ups_state[modelnum].word3.bRunMode;
    if(tempa == 0)state = "UPS";
    else if(tempa == 1)state = "逆变器";
    else state = "数据错误";
    appendOneRow(27, "模块运行模式", tempa, state, time);

    tempa = m_ups_state[modelnum].word3.bBpPriority;
    if(tempa == 0)state = "是";
    else if(tempa == 1)state = "否";
    else state = "数据错误";
    appendOneRow(28, "旁路优先", tempa, state, time);

    tempa = m_ups_state[modelnum].word3.bStartupMode;
    if(tempa == 0)state = "手动";
    else if(tempa == 1)state = "自动";
    else state = "数据错误";
    appendOneRow(29, "启机模式", tempa, state, time);
}

void datamonitoring::dispanalog(int modelnum)
{
    ui->mAnalog_pushButton->setStyleSheet("background:green");
    ui->mWaring_pushButton->setStyleSheet("");
    ui->mState_pushButton->setStyleSheet("");
    ui->tableWidget->setRowCount(21);

    float tempa = 0;
    QString time = updatatime[modelnum];

    tempa = (float)m_ups_analog[modelnum].InPhaseVlt.u16Phase_a/10;
    appendOneRow(0, "输入电压A",tempa, "V",time);
    tempa = (float)m_ups_analog[modelnum].InPhaseVlt.u16Phase_b/10;
    appendOneRow(1, "输入电压B",tempa, "V",time);
    tempa = (float)m_ups_analog[modelnum].InPhaseVlt.u16Phase_c/10;
    appendOneRow(2, "输入电压C",tempa, "V",time);

    tempa = (float)m_ups_analog[modelnum].InFreq.u16Self/100;
    appendOneRow(3, "输入频率",tempa, "Hz",time);

    tempa = (float)m_ups_analog[modelnum].InPhaseCurr.u16Phase_a/100;
    appendOneRow(4, "输入电流A",tempa, "A",time);
    tempa = (float)m_ups_analog[modelnum].InPhaseCurr.u16Phase_b/100;
    appendOneRow(5, "输入电流B",tempa, "A",time);
    tempa = (float)m_ups_analog[modelnum].InPhaseCurr.u16Phase_c/100;
    appendOneRow(6, "输入电流C",tempa, "A",time);

    tempa = m_ups_analog[modelnum].InBatVlt.u16Self;
    if(tempa >= 32768)tempa = -((65536 - tempa)/10);
    appendOneRow(7, "电池电压",tempa, "V",time);
    tempa = m_ups_analog[modelnum].InBatCurr.u16Self/100;
    appendOneRow(8, "电池电流",tempa, "A",time);

    tempa = (float)m_ups_analog[modelnum].InBypVlt.u16Self/10;
    appendOneRow(9, "旁路电压",tempa, "V",time);
    tempa = (float)m_ups_analog[modelnum].InBypFreq.u16Self/100;
    appendOneRow(10, "旁路频率",tempa, "Hz",time);

    tempa = (float)m_ups_analog[modelnum].OutVlt.u16Self/10;
    appendOneRow(11, "输出电压",tempa, "V",time);
    tempa = (float)m_ups_analog[modelnum].OutCur.u16Self/100;
    appendOneRow(12, "输出电流",tempa, "A",time);
    tempa = (float)m_ups_analog[modelnum].OutFreq.u16Self/100;
    appendOneRow(13, "输出频率",tempa, "Hz",time);

    tempa = (float)m_ups_analog[modelnum].OutApparentPwr.u16Self;
    appendOneRow(14, "视在功率",tempa, "W",time);
    tempa = (float)m_ups_analog[modelnum].OutActPwr.u16Self;
    appendOneRow(15, "有功功率",tempa, "W",time);
    tempa = (float)m_ups_analog[modelnum].LoadRate.u16Self;
    appendOneRow(16, "负载百分比",tempa, "%",time);
    tempa = (float)m_ups_analog[modelnum].Temperature.u16Self;
    appendOneRow(17, "模块温度",tempa, "℃",time);
    tempa = m_ups_analog[modelnum].DcBusP.u16Self*380/4096;
    appendOneRow(18, "正母线电压",tempa, "V",time);
    tempa = m_ups_analog[modelnum].DcBusN.u16Self*380/4096;
    appendOneRow(19, "负母线电压",tempa, "V",time);
    tempa = (float)m_ups_analog[modelnum].AgingRate.u16Self/10;
    appendOneRow(20, "模块老化率",tempa, "%",time);
}

void datamonitoring::settableheader()
{
    ui->tableWidget->clear();

    QStringList horizontalHeader;
    horizontalHeader << "名称" <<"数值" <<"单位/状态" <<"更新时间";
    ui->tableWidget->setHorizontalHeaderLabels(horizontalHeader);
}

void datamonitoring::onealarmmonitor(QString name)
{
    int i = 0;
    float tempa = 0;
    QString time;
    QString str;

    if(name == "风扇故障"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word1.bFanFaultshow;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "风扇故障", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }
    }
    else if(name == "母线高压异常"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word1.bDCHighAbnormal;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "母线高压异常", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "逆变器故障"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word1.bInvFault;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "逆变器故障", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "过温故障"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word1.bOVTMP;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "过温故障", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "短路故障"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word1.bShortFault;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "短路故障", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "放电器故障"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word1.bDischgFault;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "放电器故障", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "整流器故障"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word1.bRecFault;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "整流器故障", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "电池反接"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word1.bBattStUpInverse;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "电池反接", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "直流母线电压低"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word1.bDCLowAbnormal;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "直流母线电压低", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "硬件检测母线电压高"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word1.bBusOVPAbnormal;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "硬件检测母线电压高", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "输出过载延时到"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word1.bInvOvloadTimeout;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "输出过载延时到", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "输出过载告警标志"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word1.bOvload;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "输出过载告警标志", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "电池电压低"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word1.bBattLowFault;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "电池电压低", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "电池高压故障"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word1.bBattHighFault;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "电池高压故障", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "主机CANA故障"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word1.bMasterCANAFault;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "主机CANA故障", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "CANA故障"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word1.bCANAFault;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "CANA故障", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }

    else if(name == "主路正常"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word2.bMainOk;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "主路正常", tempa, str, time);
            if(tempa != 1)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "交流电压"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word2.bVin;
            if(tempa == 0)str = "正常";
            else if(tempa == 1)str = "低异常";
            else if(tempa == 2)str = "欠压";
            else if(tempa == 4)str = "过压";
            else if(tempa == 8)str = "高异常";
            else str = "数据错误";
            appendOneRow(i, "交流电压", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "交流频率"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word2.bFin;
            if(tempa == 0)str = "正常";
            else if(tempa == 1)str = "低异常";
            else if(tempa == 2)str = "高异常";
            else str = "数据错误";
            appendOneRow(i, "交流频率", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "旁路电压"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word2.bVbp;
            if(tempa == 0)str = "正常";
            else if(tempa == 1)str = "低异常";
            else if(tempa == 2)str = "高异常";
            else str = "数据错误";
            appendOneRow(i, "旁路电压", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "旁路频率"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word2.bFbp;
            if(tempa == 0)str = "正常";
            else if(tempa == 1)str = "低异常";
            else if(tempa == 2)str = "高异常";
            else str = "数据错误";
            appendOneRow(i, "旁路频率", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "并机地址冲突"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word2.bParaAddOverlap;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "并机地址冲突", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "切换次数到"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word2.bSwitchTimesClamp;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "切换次数到", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "均流故障"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word2.bParaCurShareFault;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "均流故障", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "放电器软起故障"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word2.bDischgFaultSoftSt;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "放电器软起故障", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "放电器直流母线高"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word3.bDischgFaultDCbusHigh;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "放电器直流母线高", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "放电器直流母线低"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word3.bDischgFaultDCbusLow;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "放电器直流母线低", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "放电器直流母线短路"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word3.bDischgFaultDCbusShort;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "放电器直流母线短路", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "整流直流母线高"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word3.bRecFaultHigh;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "整流直流母线高", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "整流直流母线低"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word3.bRecFaultLow;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "整流直流母线低", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "被动软起失败"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word3.bPassiveSSDCAbnormal;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "被动软起失败", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "主动软起失败"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word3.bActiveSSDCAbnormal;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "主动软起失败", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "辅助电源故障"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word3.bAuxPowerFault;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "辅助电源故障", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "高频同步信号故障"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word3.bCarrierSyncFault;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "高频同步信号故障", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "工频同步信号故障"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word3.bInvLowSyncFault;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "工频同步信号故障", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else if(name == "单三相故障"){
        for ( i = 0; i < MODE_NUM; ++i) {
            time = updatatime[i];

            tempa = m_ups_waring[i].word3.bPhaseOutFault;
            if(tempa == 0)str = "否";
            else if(tempa == 1)str = "是";
            else str = "数据错误";
            appendOneRow(i, "单三相故障", tempa, str, time);
            if(tempa != 0)ui->tableWidget->item(i, 2)->setBackground(Qt::red);
        }

    }
    else{
        QMessageBox::critical(this,"选择错误","请选择正确的选项",QMessageBox::Ok);
    }
}

void datamonitoring::onestatemonitor(QString name)
{
    int i;
    float tempa = 0;
    QString time;
    QString state;

    if(name == "维修空开状态"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word1.bDcBusPreSoftStIng;
            if(tempa == 0)state = "断开";
            else if(tempa == 1)state = "闭合";
            else state = "数据错误";
            appendOneRow(i, "维修空开状态", tempa, state, time);
        }
    }
    else if(name == "母线软起成功"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word1.bDcBusSoftStOver;
            if(tempa == 0)state = "否";
            else if(tempa == 1)state = "是";
            else state = "数据错误";
            appendOneRow(i, "母线软起成功", tempa, state, time);
        }
    }
    else if(name == "放电器状态"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word1.bDischg;
            if(tempa == 0)state = "关闭";
            else if(tempa == 1)state = "被动软启动";
            else if(tempa == 2)state = "主动软启动";
            else if(tempa == 3)state = "启动完毕";
            else state = "数据错误";
            appendOneRow(i, "放电器状态", tempa, state, time);
        }
    }
    else if(name == "PFC状态"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word1.bPFC;
            if(tempa == 0)state = "关闭";
            else if(tempa == 1)state = "软启动";
            else if(tempa == 3)state = "启动完毕";
            else state = "数据错误";
            appendOneRow(i, "PFC状态", tempa, state, time);
        }
    }
    else if(name == "逆变器状态"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word1.bInv;
            if(tempa == 0)state = "关闭";
            else if(tempa == 1)state = "软启动";
            else if(tempa == 3)state = "启动完毕";
            else state = "数据错误";
            appendOneRow(i, "逆变器状态", tempa, state, time);
        }
    }
    else if(name == "主动软起中"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word1.bDcBusPreSoftStIng;
            if(tempa == 0)state = "否";
            else if(tempa == 1)state = "是";
            else state = "数据错误";
            appendOneRow(i, "主动软起中", tempa, state, time);
        }
    }
    else if(name == "主动软启动完成"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word1.bDcBusPreSoftStOver;
            if(tempa == 0)state = "否";
            else if(tempa == 1)state = "是";
            else state = "数据错误";
            appendOneRow(i, "主动软启动完成", tempa, state, time);
        }
    }
    else if(name == "供电状态"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word1.bSupply;
            if(tempa == 0)state = "均不供电";
            else if(tempa == 1)state = "旁路供电";
            else if(tempa == 2)state = "逆变供电";
            else state = "数据错误";
            appendOneRow(i, "供电状态", tempa, state, time);
        }
    }
    else if(name == "输入相序"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word1.bSnglPhsIn;
            if(tempa == 0)state = "三相输入";
            else if(tempa == 1)state = "单相输入";
            else state = "数据错误";
            appendOneRow(i, "输入相序", tempa, state, time);
        }
    }
    else if(name == "消音状态"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word1.bSilentBeep;
            if(tempa == 0)state = "否";
            else if(tempa == 1)state = "是";
            else state = "数据错误";
            appendOneRow(i, "消音状态", tempa, state, time);
        }
    }
    else if(name == "相位跟踪标志"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word2.bPhaseTrace;
            if(tempa == 0)state = "本振";
            else if(tempa == 1)state = "跟旁路";
            else if(tempa == 2)state = "跟输出（预留）";
            else if(tempa == 3)state = "跟LBS源（预留）";
            else state = "数据错误";
            appendOneRow(i, "相位跟踪标志", tempa, state, time);
        }
    }
    else if(name == "放电器开机允许状态"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word2.bDischOnEnable;
            if(tempa == 0)state = "不允许";
            else if(tempa == 1)state = "允许";
            else state = "数据错误";
            appendOneRow(i, "放电器开机允许状态", tempa, state, time);
        }
    }
    else if(name == "整流器开机允许状态"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word2.bRecOnEnable;
            if(tempa == 0)state = "不允许";
            else if(tempa == 1)state = "允许";
            else state = "数据错误";
            appendOneRow(i, "整流器开机允许状态", tempa, state, time);
        }
    }
    else if(name == "直流电压等级"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word2.bBatLevel;
            if(tempa == 0)state = "110V";
            else if(tempa == 1)state = "220V";
            else if(tempa == 2)state = "750V";
            else state = "数据错误";
            appendOneRow(i, "直流电压等级", tempa, state, time);
        }
    }
    else if(name == "自老化模式"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word2.bAgingMode;
            if(tempa == 0)state = "正常模式";
            else if(tempa == 1)state = "自老化模式";
            else state = "数据错误";
            appendOneRow(i, "自老化模式", tempa, state, time);
        }
    }
    else if(name == "输出单三相"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word2.bOutPhaMode;
            if(tempa == 0)state = "单相";
            else if(tempa == 1)state = "三相";
            else state = "数据错误";
            appendOneRow(i, "输出单三相", tempa, state, time);
        }
    }
    else if(name == "输出相位信息"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word2.bOutPha;
            if(tempa == 0)state = "单相A相";
            else if(tempa == 1)state = "A相";
            else if(tempa == 2)state = "B相";
            else if(tempa == 3)state = "C相";
            else state = "数据错误";
            appendOneRow(i, "输出相位信息", tempa, state, time);
        }
    }
    else if(name == "旁路供电允许"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word2.bBpSupplyPermit;
            if(tempa == 0)state = "不允许";
            else if(tempa == 1)state = "允许";
            else state = "数据错误";
            appendOneRow(i, "旁路供电允许", tempa, state, time);
        }
    }
    else if(name == "输出频率自适应"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word2.bOutFinAdaptive;
            if(tempa == 0)state = "不自适应";
            else if(tempa == 1)state = "自适应";
            else state = "数据错误";
            appendOneRow(i, "输出频率自适应", tempa, state, time);
        }
    }
    else if(name == "输出电压等级"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word2.bOutVoltLevel;
            if(tempa == 0)state = "220V";
            else if(tempa == 1)state = "230V";
            else if(tempa == 2)state = "240V";
            else state = "数据错误";
            appendOneRow(i, "输出电压等级", tempa, state, time);
        }
    }
    else if(name == "输出频率等级"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word2.bOutFinLevel;
            if(tempa == 0)state = "50Hz";
            else if(tempa == 1)state = "60Hz";
            else state = "数据错误";
            appendOneRow(i, "输出频率等级", tempa, state, time);
        }
    }
    else if(name == ""){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];
        }
    }
    else if(name == "旁路频率范围"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word3.bBpFinRang;
            if(tempa == 0)state = "±1Hz";
            else if(tempa == 1)state = "±2Hz";
            else if(tempa == 2)state = "±3Hz";
            else state = "数据错误";
            appendOneRow(i, "旁路频率范围", tempa, state, time);
        }
    }
    else if(name == "旁路电压范围"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word3.bBpVoltRang;
            if(tempa == 0)state = "±10%";
            else if(tempa == 1)state = "±15%";
            else state = "数据错误";
            appendOneRow(i, "旁路电压范围", tempa, state, time);
        }
    }
    else if(name == "输出功率状态"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word3.bOutPower;
            if(tempa == 0)state = "3KWA";
            else if(tempa == 1)state = "5KWA";
            else state = "数据错误";
            appendOneRow(i, "输出功率状态", tempa, state, time);
        }
    }
    else if(name == "休眠模式"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word3.bSleepMode;
            if(tempa == 0)state = "否";
            else if(tempa == 1)state = "是";
            else state = "数据错误";
            appendOneRow(i, "休眠模式", tempa, state, time);
        }
    }
    else if(name == "模块自检"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word3.bSelfTest;
            if(tempa == 0)state = "否";
            else if(tempa == 1)state = "是";
            else state = "数据错误";
            appendOneRow(i, "模块自检", tempa, state, time);
        }
    }
    else if(name == "旁路模式"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word3.bBpMode;
            if(tempa == 0)state = "否";
            else if(tempa == 1)state = "是";
            else state = "数据错误";
            appendOneRow(i, "旁路模式", tempa, state, time);
        }
    }
    else if(name == "模块运行模式"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word3.bRunMode;
            if(tempa == 0)state = "UPS";
            else if(tempa == 1)state = "逆变器";
            else state = "数据错误";
            appendOneRow(i, "模块运行模式", tempa, state, time);
        }
    }
    else if(name == "旁路优先"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word3.bBpPriority;
            if(tempa == 0)state = "是";
            else if(tempa == 1)state = "否";
            else state = "数据错误";
            appendOneRow(i, "旁路优先", tempa, state, time);
        }
    }
    else if(name == "启机模式"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];

            tempa = m_ups_state[i].word3.bStartupMode;
            if(tempa == 0)state = "手动";
            else if(tempa == 1)state = "自动";
            else state = "数据错误";
            appendOneRow(i, "启机模式", tempa, state, time);
        }
    }
    else{
        QMessageBox::critical(this,"选择错误","请选择正确的选项",QMessageBox::Ok);
    }
}

void datamonitoring::oneanalogmonitor(QString name)
{
    int i;
    float tempa = 0;
    QString time;

    if(name == "输入电压A"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];
            tempa = (float)m_ups_analog[i].InPhaseVlt.u16Phase_a/10;
            appendOneRow(i,"输入电压A",tempa, "V",time);
        }
    }
    else if(name == "输入电压B"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];
            tempa = (float)m_ups_analog[i].InPhaseVlt.u16Phase_b/10;
            appendOneRow(i, "输入电压B",tempa, "V",time);
        }
    }
    else if(name == "输入电压C"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];
            tempa = (float)m_ups_analog[i].InPhaseVlt.u16Phase_c/10;
            appendOneRow(i, "输入电压C",tempa, "V",time);
        }
    }
    else if(name == "输入频率"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];
            tempa = (float)m_ups_analog[i].InFreq.u16Self/100;
            appendOneRow(i, "输入频率",tempa, "Hz",time);
        }
    }
    else if(name == "输入电流A"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];
            tempa = (float)m_ups_analog[i].InPhaseCurr.u16Phase_a/100;
            appendOneRow(i, "输入电流A",tempa, "A",time);
        }
    }
    else if(name == "输入电流B"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];
            tempa = (float)m_ups_analog[i].InPhaseCurr.u16Phase_b/100;
            appendOneRow(i, "输入电流B",tempa, "A",time);
        }
    }
    else if(name == "输入电流C"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];
            tempa = (float)m_ups_analog[i].InPhaseCurr.u16Phase_c/100;
            appendOneRow(i, "输入电流C",tempa, "A",time);
        }
    }
    else if(name == "电池电压"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];
            tempa = m_ups_analog[i].InBatVlt.u16Self;
            if(tempa >= 32768)tempa = -((65536 - tempa)/10);
            appendOneRow(i, "电池电压",tempa, "V",time);
        }
    }
    else if(name == "电池电流"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];
            tempa = (float)m_ups_analog[i].InBatCurr.u16Self/100;
            appendOneRow(i, "电池电流",tempa, "A",time);
        }
    }
    else if(name == "旁路电压"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];
            tempa = (float)m_ups_analog[i].InBypVlt.u16Self/10;
            appendOneRow(i, "旁路电压",tempa, "V",time);
        }
    }
    else if(name == "旁路频率"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];
            tempa = (float)m_ups_analog[i].InBypFreq.u16Self/100;
            appendOneRow(i, "旁路频率",tempa, "Hz",time);
        }
    }
    else if(name == "输出电压"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];
            tempa = (float)m_ups_analog[i].OutVlt.u16Self/10;
            appendOneRow(i, "输出电压",tempa, "V",time);
        }
    }
    else if(name == "输出电流"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];
            tempa = (float)m_ups_analog[i].OutCur.u16Self/100;
            appendOneRow(i, "输出电流",tempa, "A",time);
        }
    }
    else if(name == "输出频率"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];
            tempa = (float)m_ups_analog[i].OutFreq.u16Self/100;
            appendOneRow(i, "输出频率",tempa, "Hz",time);
        }
    }
    else if(name == "视在功率"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];
            tempa = (float)m_ups_analog[i].OutApparentPwr.u16Self;
            appendOneRow(i, "视在功率",tempa, "W",time);
        }
    }
    else if(name == "有功功率"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];
            tempa = (float)m_ups_analog[i].OutActPwr.u16Self;
            appendOneRow(i, "有功功率",tempa, "W",time);
        }
    }
    else if(name == "负载百分比"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];
            tempa = (float)m_ups_analog[i].LoadRate.u16Self;
            appendOneRow(i, "负载百分比",tempa, "%",time);
        }
    }
    else if(name == "模块温度"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];
            tempa = (float)m_ups_analog[i].Temperature.u16Self;
            appendOneRow(i, "模块温度",tempa, "℃",time);
        }
    }
    else if(name == "正母线电压"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];
            tempa = m_ups_analog[i].DcBusP.u16Self*380/4096;
            appendOneRow(i, "正母线电压",tempa, "V",time);
        }
    }
    else if(name == "负母线电压"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];
            tempa = m_ups_analog[i].DcBusN.u16Self*380/4096;
            appendOneRow(i, "负母线电压",tempa, "V",time);
        }
    }
    else if(name == "模块老化率AgingRate"){
        for ( i = 0; i < MODE_NUM-1; ++i) {
            time = updatatime[i];
            tempa = (float)m_ups_analog[i].AgingRate.u16Self/10;
            appendOneRow(i, "模块老化率",tempa, "%",time);
        }
    }
    else{
        QMessageBox::critical(this,"选择错误","请选择正确的选项",QMessageBox::Ok);
    }
}

void datamonitoring::on_mOpenfile_pushButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("打开"),"",("*.txt"));
    if(filePath.isEmpty()){
        QMessageBox::warning(this,"打开失败","文件路径为空");
        return ;
    }
    ui->lineEdit->setText(filePath);

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"文件打开失败";
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        qDebug() << line;
    }
}

void datamonitoring::on_mSavefile_pushButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存"),"","Excel 文件(*.xlsx *.xls)");
    if (fileName!="")
    {
        QAxObject *excel = new QAxObject;
        if (excel->setControl("Excel.Application")) //连接Excel控件
        {
            excel->dynamicCall("SetVisible (bool Visible)","false");//不显示窗体
            //不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示
            excel->setProperty("DisplayAlerts", false);
            QAxObject *workbooks = excel->querySubObject("WorkBooks");//获取工作簿集合
            workbooks->dynamicCall("Add");//新建一个工作簿
            QAxObject *workbook = excel->querySubObject("ActiveWorkBook");//获取当前工作簿
            QAxObject *worksheet = workbook->querySubObject("Worksheets(int)", 1);

            int i,j;
            int colcount=ui->tableWidget->columnCount();
            int rowscount= ui->tableWidget->rowCount();

            QAxObject *cell,*col;
            //标题行
            cell=worksheet->querySubObject("Cells(int,int)", 1, 1);
            cell->dynamicCall("SetValue(const QString&)", "IPM4X模拟量数据记录");
            cell->querySubObject("Font")->setProperty("Size", 18);
            //调整行高
            worksheet->querySubObject("Range(const QString&)", "1:1")->setProperty("RowHeight", 30);
            //合并标题行
            QString cellTitle;
            cellTitle.append("A1:");
            cellTitle.append(QChar(colcount - 1 + 'A'));
            cellTitle.append(QString::number(1));
            QAxObject *range = worksheet->querySubObject("Range(const QString&)", cellTitle);
            range->setProperty("WrapText", true);
            range->setProperty("MergeCells", true);
            range->setProperty("HorizontalAlignment", -4108);//xlCenter
            range->setProperty("VerticalAlignment", -4108);//xlCenter
            //列标题
            for(i=0;i<colcount;i++)
            {
                QString columnName;
                columnName.append(QChar(i  + 'A'));
                columnName.append(":");
                columnName.append(QChar(i + 'A'));
                col = worksheet->querySubObject("Columns(const QString&)", columnName);
                col->setProperty("ColumnWidth", ui->tableWidget->columnWidth(i)/6);
                cell=worksheet->querySubObject("Cells(int,int)", 2, i+1);
                //QTableWidget 获取表格头部文字信息
                columnName=ui->tableWidget->horizontalHeaderItem(i)->text();
                //QTableView 获取表格头部文字信息
                // columnName=ui->tableView_right->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
                cell->dynamicCall("SetValue(const QString&)", columnName);
                cell->querySubObject("Font")->setProperty("Bold", true);
                cell->querySubObject("Interior")->setProperty("Color",QColor(191, 191, 191));
                cell->setProperty("HorizontalAlignment", -4108);//xlCenter
                cell->setProperty("VerticalAlignment", -4108);//xlCenter
            }

            //QTableWidget 获取表格数据部分
            for(i=0;i<rowscount;i++){
                for (j=0;j<colcount;j++)
                {
                    worksheet->querySubObject("Cells(int,int)",i+3,j+1)
                            ->dynamicCall("SetValue(const QString&)",
                                          ui->tableWidget->item(i,j)?ui->tableWidget->item(i,j)->text():"");
                }
            }

            //画框线
            QString lrange;
            lrange.append("A2:");
            lrange.append((QChar)(colcount - 1+ 'A'));
            lrange.append(QString::number(ui->tableWidget->rowCount() + 2));
            range = worksheet->querySubObject("Range(const QString&)", lrange);
            range->querySubObject("Borders")->setProperty("LineStyle", QString::number(1));
            range->querySubObject("Borders")->setProperty("Color", QColor(0, 0, 0));
            //调整数据区行高
            QString rowsName;
            rowsName.append("2:");
            rowsName.append(QString::number(ui->tableWidget->rowCount() + 2));
            range = worksheet->querySubObject("Range(const QString&)", rowsName);
            range->setProperty("RowHeight", 20);
            workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(fileName));//保存至fileName
            workbook->dynamicCall("Close()");//关闭工作簿
            excel->dynamicCall("Quit()");//关闭excel
            delete excel;
            excel=NULL;
            if (QMessageBox::question(NULL,"完成","文件已经导出，是否现在打开？",
                                      QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
            {
                QDesktopServices::openUrl(QUrl("file:///" + QDir::toNativeSeparators(fileName)));
            }
        }
        else
        {
            QMessageBox::warning(NULL,"错误","未能创建 Excel 对象，请安装 Microsoft Excel。",QMessageBox::Apply);
        }
    }

}

void datamonitoring::on_mWaring_pushButton_clicked()
{
    settableheader();
    distype = ALARM_TPYE;
    dispalarm(ui->modenum_comboBox->currentIndex());

    ui->datatype_comboBox->clear();
    QList<QString> name;
    //word1
    name <<"单模块变量查询" <<"风扇故障" <<"母线高压异常" <<"逆变器故障" <<"过温故障" <<"短路故障"
        <<"放电器故障" <<"整流器故障"<<"电池反接"<<"直流母线电压低" <<"硬件检测母线电压高"
       <<"输出过载延时到" <<"输出过载告警标志"<<"电池电压低" <<"电池高压故障" <<"主机CANA故障" <<"CANA故障";
    //word2
    name <<"主路正常" <<"交流电压" <<"交流频率" <<"旁路电压"
        <<"旁路频率" <<"并机地址冲突" <<"切换次数到" <<"均流故障" <<"放电器软起故障";
    //word3
    name <<"放电器直流母线高" <<"放电器直流母线低" <<"放电器直流母线短路" <<"整流直流母线高"
        <<"整流直流母线低" <<"被动软起失败" <<"主动软起失败" <<"辅助电源故障"
       <<"高频同步信号故障" <<"工频同步信号故障" <<"单三相故障";
    ui->datatype_comboBox->addItems(name);
    ui->datatype_comboBox->adjustItemWidth();
}

void datamonitoring::on_mState_pushButton_clicked()
{
    settableheader();
    distype = STATE_TPYE;
    dispsate(ui->modenum_comboBox->currentIndex());

    ui->datatype_comboBox->clear();
    QList<QString> name;
    //word1
    name <<"单模块变量查询" <<"维修空开状态" <<"母线软起成功" <<"放电器状态" <<"PFC状态" <<"逆变器状态"
        <<"主动软起中" <<"主动软启动完成" <<"供电状态" <<"输入相序" <<"消音状态";
    //word2
    name <<"相位跟踪标志" <<"放电器开机允许状态" <<"整流器开机允许状态" <<"直流电压等级" <<"自老化模式"
        <<"输出单三相" <<"输出相位信息" <<"旁路供电允许" <<"输出频率自适应" <<"输出电压等级" <<"输出频率等级";
    //word3
    name <<"旁路频率范围" <<"旁路电压范围" <<"输出功率状态" <<"休眠模式" <<"模块自检" <<"旁路模式"
        <<"模块运行模式" <<"旁路优先" <<"启机模式";

    ui->datatype_comboBox->addItems(name);
    ui->datatype_comboBox->adjustItemWidth();

}

void datamonitoring::on_mAnalog_pushButton_clicked()
{
    settableheader();
    distype = ANALOG_TPYE;
    dispanalog(ui->modenum_comboBox->currentIndex());

    ui->datatype_comboBox->clear();
    QList<QString> name;
    name <<"单模块变量查询" <<"输入电压A" <<"输入电压B" <<"输入电压C" <<"输入频率"
        <<"输入电流A" <<"输入电流B" <<"输入电流C"<<"电池电压" <<"电池电流" <<"旁路电压"
       <<"旁路频率" <<"输出电压" <<"输出电流" <<"输出频率"<<"视在功率" <<"有功功率"
      <<"负载百分比" <<"正母线电压" <<"负母线电压" <<"模块老化率AgingRate";
    ui->datatype_comboBox->addItems(name);
    ui->datatype_comboBox->adjustItemWidth();
}

void datamonitoring::on_datatype_comboBox_textActivated(const QString &arg1)
{
    if(arg1 == "单模块变量查询"){
        ui->modenum_comboBox->setEnabled(true);
    }
    else{
        ui->modenum_comboBox->setEnabled(false);
    }

    if(distype == 0){
        QMessageBox::critical(this,"错误","请先选择查看类型",QMessageBox::Ok);
        return;
    }

    ui->tableWidget->setRowCount(24);
    if(distype == ALARM_TPYE){
        onealarmmonitor(arg1);
    }else if(distype == STATE_TPYE){
        onestatemonitor(arg1);
    }else if(distype == ANALOG_TPYE){
        oneanalogmonitor(arg1);
    }else{
        //Do Nothing
    }
}

void datamonitoring::on_modenum_comboBox_activated(int index)
{
    if(distype == 0){
        QMessageBox::critical(this,"错误","请先选择查看类型",QMessageBox::Ok);
        return;
    }

    qDebug() << index;
    if(distype == ALARM_TPYE){
        dispalarm(index);
    }else if(distype == STATE_TPYE){
        dispsate(index);
    }else if(distype == ANALOG_TPYE){
        dispanalog(index);
    }else{
        //Do Nothing
    }
}

void datamonitoring::on_msql_pushButton_clicked()
{
    ui_Sql->show();
}

void datamonitoring::on_openacset_pushButton_clicked()
{
    emit open_acui();
}

void datamonitoring::pollpace(int n)
{
    ui->mpoll_progressBar->setValue(n);
    ui->mpoll_progressBar->update();
}

void datamonitoring::on_m_uishow_pushButton_clicked()
{
    emit mainshow();
}

