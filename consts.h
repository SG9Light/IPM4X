#ifndef CONSTS_H
#define CONSTS_H

/***START IPM4205 MODEBUS命令相关宏定义***/
//并机最大台数
#define RXDATAMAXSIZE    64

//并机最大台数
#define MODE_NUM    24
//功能码
#define FUNTION_QUERY_CODE           0x03   //查询码
#define FUNTION_CONTROL_CODE         0x06   //控制码

//基本信息
#define COMM_REG_ADDR               0x03E8	//通信地址查询寄存器           1000
#define DSP_VER_REG_ADDR			0x03EE	//DSP软件版本查询寄存器        1006
#define MON_VER_REG_ADDR			0x03EF	//显示软件版本查询寄存器        1007
#define HARD_VER_REG_ADDR			0x03F0	//硬件版本查询寄存器           1008
//告警量
#define ALARM_TPYE                  1
#define ALARM_LEN                   0x0003
#define WARN1_REG_ADDR				0x044C	//告警量1查询寄存器			1100
#define WARN2_REG_ADDR				0x044D	//告警量2查询寄存器			1101
#define WARN3_REG_ADDR				0x044E	//告警量3查询寄存器            1102
//状态量
#define STATE_TPYE                  2
#define STATE_LEN                   0x0003
#define STATUS1_REG_ADDR			0x04B0	//状态量1查询寄存器			1200
#define STATUS2_REG_ADDR			0x04B1	//状态量2查询寄存器			1201
#define STATUS3_REG_ADDR			0x04B2	//状态量3查询寄存器			1202
//模拟量
#define ANALOG_TPYE                  3
#define ANALOG_LEN                  0x0015
#define IN_VOLT_A_REG_ADDR			0x0514	//输入电压A查询寄存器			1300
#define IN_VOLT_B_REG_ADDR			0x0515	//输入电压B查询寄存器			1301
#define IN_VOLT_C_REG_ADDR			0x0516	//输入电压C查询寄存器			1302
#define IN_FREQ_REG_ADDR			0x0517	//输入频率查询寄存器           1303
#define IN_CURR_A_REG_ADDR			0x0518	//输入电流A查询寄存器			1304
#define IN_CURR_B_REG_ADDR			0x0519	//输入电流B查询寄存器			1305
#define IN_CURR_C_REG_ADDR			0x051A	//输入电流C查询寄存器			1306
#define BAT_VOLT_REG_ADDR			0x051B	//电池电压查询寄存器			1307
#define BAT_CURR_REG_ADDR			0x051C	//电池电流查询寄存器			1308
#define BYP_VOLT_REG_ADDR			0x051D	//旁路电压查询寄存器			1309
#define BYP_FREQ_REG_ADDR			0x051E	//旁路频率查询寄存器			1310
#define OUT_VOLT_REG_ADDR			0x051F	//输出电压查询寄存器			1311
#define OUT_CURR_REG_ADDR			0x0520	//输出电流查询寄存器			1312
#define OUT_FREQ_REG_ADDR			0x0521	//输出频率查询寄存器			1313
#define APP_POWER_REG_ADDR			0x0522	//视在功率查询寄存器			1314
#define ACT_POWER_REG_ADDR			0x0523	//有功功率查询寄存器			1315
#define LOAD_PRE_REG_ADDR			0x0524	//负载百分比查询寄存器          1316
#define TEMP_REG_ADDR                0x0525	//温度查询寄存器               1317
//设置量
#define COMM_ADDR_SET_REG_ADDR      0x0578	//通信地址设置                1400
#define PAR_ADDR_SET_REG_ADDR       0x0579	//并机地址设置					1401
#define PHASE_SET_REG_ADDR			0x0583	//单三相设置					1411
#define BYP_EN_REG_ADDR				0x0584	//旁路使能设置					1412
#define BYP_FIRST_REG_ADDR			0x0585	//旁路优先设置					1413
#define STARTUP_MODE_REG_ADDR       0x0587	//起机模式设置					1415
//控制量
#define RUN_SHUT_REG_ADDR_SEND   0x05DE	//远程开关机						1502

/***START IPM4205 定时处理函数表***/
#define CMD_PRD_TABL_SIZE   3

enum
{
    System = 1,
    Alarm,
    State,
    Analog,
    Steing,
    Control
};

enum
{
    DESTINATION,
    FUNCODE,
    ADDR_HIGHT,
    ADDR_LOW,
    DATA_HIGHT,
    DATA_LOW
};


#endif // CONSTS_H
