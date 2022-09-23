#ifndef TYPES_H
#define TYPES_H

#include "consts.h"
#include <cstdint>
#include <QObject>

//串口接收数据
typedef struct{
    uint8_t len;
    uint8_t messagetype;
    uint8_t data[RXDATAMAXSIZE];

}_PORT_RXMSG;

//告警量
struct waringword1{
    uint16_t	bFanFaultshow:1;
    /*		BIT00    风扇故障告警
                0 无
                1 有
    */
    uint16_t	bDCHighAbnormal:1;
    /*		BIT01	母线高压异常
                0 无
                1 有
    */
    uint16_t	bInvFault:1;
    /*		BIT02	逆变器故障
                0 无
                1 有
    */
    uint16_t	bOVTMP:1;
    /*		BIT03    过温故障
                0 无
                1 有
    */
    uint16_t	bShortFault:1;
    /*		BIT04  	 短路故障
                0 无
                1 有
    */
    uint16_t	bDischgFault:1;
    /*		BIT05  	 放电器故障
                0 无
                1 有
    */
    uint16_t	bRecFault:1;
    /*		BIT06    整流器故障
                0 无
                1 有
    */
    uint16_t	bBattStUpInverse:1;
    /*		BIT07    电池反接
                0 无
                1 有
    */
    uint16_t	bDCLowAbnormal:1;
    /*		BIT08    直流母线电压低
                0 无
                1 有
    */
    uint16_t	bBusOVPAbnormal:1;
    /*		BIT09	硬件检测母线电压高
                0 无
                1 有
    */
    uint16_t	bInvOvloadTimeout:1;
    /*		BIT10  	 输出过载延时到
                0 无
                1 有
    */
    uint16_t	bOvload:1;
    /*		BIT11  	 输出过载告警标志
                0 无
                1 有
    */
    uint16_t	bBattLowFault:1;
    /*		BIT12	电池电压低
                0 正常
                1 低
    */
    uint16_t	bBattHighFault:1;
    /*		BIT13  	 电池高压故障
                0 无
                1 有
    */
    uint16_t	bMasterCANAFault:1;
    /*			BIT14  主机CANA故障
                0 无
                1 是
    */
    uint16_t	bCANAFault:1;
    /*			BIT15  CANA故障
                0 无
                1 是
    */

};

struct waringword2{
    uint16_t	bMainOk:1;
    /*		BIT00		 主路正常
                0 无
                1 有
    */
    uint16_t	bVin:3;
    /*		BIT01-03 电压标志
            0000	正常
            0001	低异常
            0010	欠压
            0100	过压
            1000	高异常
    */
    uint16_t	bRev4:1;
    /*		BIT04 保留
            00  正常
            01  异常
    */
    uint16_t	bFin:2;
    /*		BIT05-06 频率标志
            00  正常
            01	低异常
            10	高异常
    */
    uint16_t	bVbp:2;
    /*		BIT07-08  旁路电压范围
            00	正常
            01	电压低
            10	电压高
    */
    uint16_t	bFbp:2;
    /*		BIT09-10 频率标志
            00  正常
            01	低异常
            10	高异常
    */
    uint16_t	bRev11:1;
    /*		BIT11 保留
            00  正常
            01  异常
    */
    uint16_t 	bParaAddOverlap:1;
    /*		BIT12	并机地址冲突
            0-无
            1-有
    */
    uint16_t 	bSwitchTimesClamp:1;
    /*		BIT13	切换次数到
            0-无
            1-有
    */
    uint16_t 	bParaCurShareFault:1;
    /*		BIT14	均流故障
            0-无
            1-有
    */
    uint16_t	bDischgFaultSoftSt:1;
    /*		BIT15	放电器软起故障
            0 无
            1 有
    */

};

struct waringword3{
    uint16_t	bDischgFaultDCbusHigh:1;
    /*		BIT00	放电器直流母线高
            0 无
            1 有
    */
    uint16_t	bDischgFaultDCbusLow:1;
    /*		BIT01	放电器直流母线电压低
            0 无
            1 有
    */
    uint16_t	bDischgFaultDCbusShort:1;
    /*		BIT02	放电器直流母线短路
            0 无
            1 有
    */
    uint16_t	bRecFaultHigh:1;
    /*		BIT03	整流直流母线高
            0 无
            1 有
    */
    uint16_t	bRecFaultLow:1;
    /*		BIT04	整流直流母线低
            0 无
            1 有
    */
    uint16_t	bPassiveSSDCAbnormal:1;
    /*		BIT05	被动软起失败
            0 无
            1 有
    */
    uint16_t	bActiveSSDCAbnormal:1;
    /*		BIT06	主动软起失败
            0 无
            1 有
    */
    uint16_t	bAuxPowerFault:1;
    /*		BIT07	辅助电源故障
            0 无
            1 有
    */
    uint16_t	bCarrierSyncFault:1;
    /*		BIT08	高频同步信号故障
            0 无
            1 有
    */
    uint16_t	bInvLowSyncFault:1;
    /*		BIT09	工频同步信号故障
            0 无
            1 有
    */
    uint16_t	bRev10:1;
    /*		BIT10 保留
            00  正常
            01  异常
    */
    uint16_t	bPhaseOutFault:1;
    /*		BIT11	单三相故障
            0 无
            1 有
    */
    uint16_t	bRev12_15:4;
    /*		BIT11	保留
            0 无
            1 有
    */
};
typedef struct{
    waringword1 word1;
    waringword2 word2;
    waringword3 word3;

}_UPS_WARING_T;
extern _UPS_WARING_T m_ups_waring[MODE_NUM];


//状态量
struct stateword1{
    uint16_t	bMaintainBpBrker:1;
    /*		BIT00	 维修空开状态
            0 断开
            1 闭合
    */
    uint16_t	bRev1:1;
    /*		BIT01	保留
            0 无
            1 有
    */
    uint16_t  bDcBusSoftStOver:1;
    /*    BIT02  母线软起成功
            0 否
            1 是

    */
    uint16_t	bDischg:2;
    /*		BIT03-04 放电器状态
            00	关闭
            01	被动软启动
            10	主动软启动
            11	启动完毕
    */
    uint16_t	bPFC:2;
    /*		BIT05-06 PFC状态
            00	关闭
            01	软启动
            11	启动完毕
    */
    uint16_t	bInv:2;
    /*		BIT07-08 逆变器状态
            00	关闭
            01	软启动
            11	启动完毕
    */
    uint16_t	bDcBusPreSoftStIng:1;
    /*		BIT09 	主动软起中
            00	否
            01	是
    */
    uint16_t	bDcBusPreSoftStOver:1;
    /*		BIT10 主动软启动完成
            00	否
            01	是
    */
    uint16_t	bSupply:2;
    /*		BIT11-12  供电状态
            00	均不供电
            01	旁路供电
            10	逆变供电
    */
    uint16_t	bSnglPhsIn:1;
    /*		BIT13 输入相序
            00	三相输入
            01	单相输入
    */
    uint16_t	bSilentBeep:1;
    /*		BIT14    消音状态
            00 否
            01 是
    */
    uint16_t	bRev15:1;
    /*		BIT15	保留
            0 无
            1 有
    */

};

struct stateword2{
    uint16_t	bPhaseTrace:2;
    /*		BIT00-01 相位跟踪标志
                00	本振
                01	跟旁路
                10	跟输出（预留）
                11	跟LBS源（预留）
    */
    uint16_t	bDischOnEnable:1;
    /*		BIT02 放电器开机允许状态
            00	不允许
            01	允许
    */
    uint16_t	bRecOnEnable:1;
    /*		BIT03 整流器开机允许状态
            00	不允许
            01	允许
    */
    uint16_t	bBatLevel:2;
    /*		BIT04-05  直流电压等级
            00	110V
            01	220B
            10	750V
    */
    uint16_t	bAgingMode:1;
    /*		BIT06 自老化模式
            00	正常
            01	自老化模式
    */
    uint16_t	bOutPhaMode:1;
    /*		BIT07 输出单三相
            00	单相
            01	三相
    */
    uint16_t	bOutPha:2;
    /*		BIT08-09 输出相位信息
            00	单相A相
            01	A相
            10  B相
            11  C相
    */
    uint16_t	bBpSupplyPermit:1;
    /*		BIT10  	旁路供电允许
                0 不允许
                1 允许
    */
    uint16_t	bOutFinAdaptive:1;
    /*		BIT11 输出频率自适应
            00	不自适应
            01	自适应
    */
    uint16_t	bOutVoltLevel:2;
    /*		BIT12-13  输出电压等级
            00	220V
            01	230V
            10	240V
    */
    uint16_t	bRev14:1;
    /*		BIT14	保留
            0 无
            1 有
    */
    uint16_t	bOutFinLevel:1;
    /*		BIT15	输出频率等级
            0 50Hz
            1 60Hz
    */

};

struct stateword3{
    uint16_t	bBpFinRang:2;
    /*		BIT00-01  旁路频率范围
            00	±1Hz
            01	±2Hz
            10	±3Hz
    */
    uint16_t	bBpVoltRang:1;
    /*		BIT02	旁路电压范围
            0 ±10%
            1 ±15%
    */
    uint16_t	bRec3:1;
    /*		BIT03	保留
            0 无
            1 有
    */
    uint16_t	bOutPower:1;
    /*		BIT04	输出功率状态
            0 3KWA
            1 5KWA
    */
    uint16_t	bSleepMode:1;
    /*		BIT05	休眠模式
            0 否
            1 是
    */
    uint16_t	bSelfTest:1;
    /*		BIT06	模块自检
            0 否
            1 是
    */
    uint16_t	bBpMode:1;
    /*		BIT07	旁路模式
            0 否
            1 是
    */
    uint16_t	bRec8_12:5;
    /*		BIT08-BIT12	保留
            0 否
            1 是
    */
    uint16_t	bRunMode:1;
    /*		BIT13	模块运行模式
            0 UPS
            1 逆变器
    */
    uint16_t	bBpPriority:1;
    /*		BIT14	旁路优先
            0 优先
            1 不优先
    */
    uint16_t	bStartupMode:1;
    /*		BIT15	启机模式
            0 手动
            1 自动
    */

};

typedef struct{
    stateword1 word1;
    stateword2 word2;
    stateword3 word3;

}_UPS_STATE_T;
extern _UPS_STATE_T m_ups_state[MODE_NUM];

//模拟量
typedef struct{
    uint16_t u16Phase_a;
    uint16_t u16Phase_b;
    uint16_t u16Phase_c;
}_PHASE_INFO_T;

typedef struct{
    uint16_t u16Self;

}_SINGLE_INFO_T;

typedef struct{
    _PHASE_INFO_T 	InPhaseVlt;			// X 10//
    _SINGLE_INFO_T  InFreq;             // X 100//
    _PHASE_INFO_T 	InPhaseCurr;		// X 100//
    //for debug
    _SINGLE_INFO_T  InBatVlt;
    _SINGLE_INFO_T  InBatCurr;
    _SINGLE_INFO_T  InBypVlt;
    _SINGLE_INFO_T 	InBypFreq;			// X 100//
    _SINGLE_INFO_T  OutVlt;             //输出电压
    _SINGLE_INFO_T  OutCur;             //输出电流
    _SINGLE_INFO_T  OutFreq;            //输出频率
    _SINGLE_INFO_T  OutApparentPwr;     //视在功率
    _SINGLE_INFO_T  OutActPwr;          //有功功率
    _SINGLE_INFO_T  LoadRate;            //负载百分比
    _SINGLE_INFO_T  Temperature;        //温度
    _SINGLE_INFO_T  DcBusP;             //母线电压+
    _SINGLE_INFO_T  DcBusN;             //母线电压-
    _SINGLE_INFO_T  AgingRate;          //老化率

}_UPS_ANALOG_T;

extern _UPS_ANALOG_T m_ups_analog[MODE_NUM];

//自定义全局变量
extern int upsmodeordinal;
extern QString updatatime[MODE_NUM];

typedef struct
{
    uint16_t    m_u16SetTickCnt; 	// The num of int ticks(Timer int) to expire timer.
    uint16_t	m_u16TickCntr;   	// Interrupt tick counter.

} TTs_PrdTmrDef;

#endif // TYPES_H
