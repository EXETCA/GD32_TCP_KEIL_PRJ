#include "main.h"
#include "tcpecho.h"
#include "lwip/opt.h"

#include <lwip/sockets.h>
#include "lwip/sys.h"
#include "lwip/api.h"
#include "task.h"

#include "System_Var.h"
#include "System_PRSRAM.h"
#include "stdarg.h"

//#include "Alarm.h"
/*水阀*/
#include "AMC7836.h"
/*报警灯*/
#include "alarm.h"
#include "Relay_Ctrl.h"
/*风扇*/
#include "fan.h"
#include "DataSamp.h"

#define Cail_Num 8
#define MAX_REC_LENGTH 30
#define TRUE 1
#define FALSE 0
/*更新程序变量*/
static struct APPDownloadCTRL App_Ctrl;

// 下位机软件版本号
const uint8_t SoftVersionString[] = "I:_DSP;V1.2_230308_CKB_HXL_XMSA";

static void FAN_RATECtrl(void *parameter);
static void FAN_CTRL(void *parameter);
static void RELAY_CTRL(void *parameter);
static void SendData(void *parameter);
static void ALE_LED_CTRL(void *parameter);
static void DeviceLED(void *parameter);
static void Gate_Control(void *parameter);
static void Smoke_State(void *parameter);
static void DEBUG_PRINT(void *parameter);
static void GetData(void *parameter);
static void SetPower_ON(void *parameter);
static void SetPower_OFF(void *parameter);
static void SetPower_Reset(void *parameter);
static void SetPower_State(void *parameter);
static void SetFan_ON(void *parameter);
static void SetFan_OFF(void *parameter);
static void SetFan_Speed(void *parameter);
static void SetValve(void *parameter);

static void GetSoftWareVersion(void *parameter);
static void OnlineFlashProgram(void *parameter);
static void ProgramUpdata(void *parameter);
static void WriteKB(void *parameter);
static void SendKB(void *parameter);
// 各个字符串命令
const CMD_Type CMD_Assemble[] = {{"WARNINGLIGHT", ALE_LED_CTRL}, // 报警灯
								                 {"MGETDATA", SendData},		 //: 上传参数
								                 {"MSETWATERVALUE", SetValve},	 // 设置水阀阀值
								                 {"MSETFANVALUE", SetFan_Speed},	 // 设置风机转速
								                 
								                 {"MSENDDATA", GetData},	 // 接收上位机下发数据
								                 {"PCALIBRATION", WriteKB},	 // 接收上位机KB，并写入Flash
								                 {"GETCALIBRATION", SendKB}, // 发送KB值到上位机
								                 
								                 // 以下命令该项目没用到								
								                 
								                 {"MGETDATADEBUG", DEBUG_PRINT}, // 上传打印数据字符串
								                 {"ANYMETHOD",},					// 万能接口 暂无
								                 {"MSETSTART", SetPower_ON},		 // 启动电源
								                 {"MSETSTOP", SetPower_OFF},		 // 停止电源
								                 {"MRESET", SetPower_Reset},		 // 复位
								                 {"MSETFANSTART", SetFan_ON},		 // 设置风机启动
								                 {"MSETFANSTOP", SetFan_OFF},		 // 设置风机停止
								                 
								                 {"MSETLIGHTCOLOR", DeviceLED},		 // 设备灯
								                 {"MSETJIMENSTATE", Gate_Control},	 // 设置磁吸门机门开关继电器
								                 {"MSETPOWERSTATE", SetPower_State}, // 设置电源开关
								                 
								                 {"GETEDITION", GetSoftWareVersion},
								                 {"ONLINEUPDATE1", OnlineFlashProgram},
								                 {"ONLINEUPDATE2", ProgramUpdata}};

static void EthernetReceiveFaiAGE(uint16_t Error);
static void EthernetReceiveSuccess(uint32_t ParameterLen, uint16_t *Send1);
static void PC_DATAHandler(char *Sptr, uint16_t len);
static uint32_t Crs(uint16_t *buf, uint16_t len);
static uint16_t Write2Mem(char *const pointer, const char *Strings, ...);

extern uint8_t Read_IP;

static uint8_t String_Verify(const char *String, char *a);

uint8_t ChanData[SINGLE_PACKAGE_BYTES] = {0}; // 发送缓存
/*更新数据*/
volatile char upgradeDATA[1052] = {0};
static struct APPDownloadCTRL App_Ctrl;

/******************************************************************
函数名称：EthernetInterfaceHandler
函数功能:以太网数据传输主程序
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void EthernetInterfaceHandler(uint16_t len)
{
	uint16_t stemp;
	uint32_t dSum, rSum;

	Eth_Data.Ethernet.ETH_Length.data = len;	   // 实际接收长度
	Eth_Data.Ethernet.ETH_ReceiveLength.data /= 2; // 字数等于字节数的一半 预备发送长度

	if (Eth_Data.Ethernet.ETH_ReceiveLength.data > 1536)
	{
		EthernetReceiveFaiAGE(0);
		return;
	}
	if (Eth_Data.Ethernet.Data[Eth_Data.Ethernet.ETH_ReceiveLength.data - 1] == 0x5E5D && Eth_Data.Ethernet.Data[Eth_Data.Ethernet.ETH_ReceiveLength.data - 2] == 0x5C5A)
	{
		dSum = Crs(Eth_Data.Ethernet.Data, Eth_Data.Ethernet.ETH_ReceiveLength.data - 4);
		rSum = Eth_Data.Ethernet.Data[Eth_Data.Ethernet.ETH_ReceiveLength.data - 4] + Eth_Data.Ethernet.Data[Eth_Data.Ethernet.ETH_ReceiveLength.data - 3] * 65536;
	}

	if (dSum == rSum) // 校验位核对
	{
		PC_DATAHandler(Eth_Data.Rec_Comand, MAX_REC_COMSIZE); // 分离命令并执行
	}
	else
	{
		EthernetReceiveFaiAGE(0);
	}
	for (stemp = 0; stemp < 1536; stemp++)
	{
		Eth_Data.Ethernet.Data[stemp] = 0;
	}
}
/********************************************************
 * @brief 数据接收成功标志，传输给上位机，或者将数据与成功标志一起上传
 * @param {uint32_t} ParameterLen 上传数据的长度
 * @param {uint16_t*} Send1  上传的数据的首地址
 * @return {void}
 * @warning 上传参数长度为零时不需要上参数首地址
 * @warning 数据长度过大时默认恢复校验和为0 ParameterLen为0时表示无上传数据，此时Send1可以为任意值
 **********************************************************/
static void EthernetReceiveSuccess(uint32_t ParameterLen, uint16_t *Send1)
{
	uint16_t temp[20], num = 0, i, length;
	uint8_t ChanHeadData[50] = {0};
	uint8_t ChanLastData[50] = {0};
	uint32_t SUM;
	uint8_t IP_TEMP = 0;
	union E_DATA_BYTE TEMP; // 缓存发送数据长度

	IP_TEMP = Read_IP;	  // 发送地址获取
	temp[num++] = 0xBBAA; // 注意顺序、返回头
	temp[num++] = 0xDDCC;
	for (i = 0; i < 10; i++) // 命令字节
	{
		temp[num++] = (Eth_Data.Rec_Comand[(2 * i + 1 + 4)] << 8) | (Eth_Data.Rec_Comand[(2 * i + 4)]);
	}
	temp[num++] = 0; // 预留字节
					 //  IP发送端地址
	temp[num++] = 0xC0A8;
	temp[num++] = (0x7B << 8) + IP_TEMP;

	temp[num++] = 0x3388; // 成功标记
						  // 计算长度 并放入发送缓存
	TEMP.data = ParameterLen * 2 + (num * 2) + 12; // 参数长度+这行之前的长度+这行之后除去命令的长度
	temp[num++] = TEMP.bytes.lbyte;
	temp[num++] = TEMP.bytes.hbyte;
	// 校验
	SUM = Crs(temp, num);
	length = num * 2;
	// 将前面计算的数据放入发送数据前端缓存
	for (i = 0; i < num; i++)
	{
		ChanHeadData[(2 * i + 1)] = (temp[i] >> 8) & 0xff;
		ChanHeadData[(2 * i)] = (temp[i]) & 0xff;
	}
	// 发送命令及回复
	write(Eth_Data.socket_descr, (&ChanHeadData[0]), length);

	// 数据发送
	if (0 != ParameterLen)
	{
		if (ParameterLen < 1536)
		{
			SUM += Crs(Send1, ParameterLen);
		}
		else
		{
			SUM = 0;
		}
		length = ParameterLen * 2;
		for (i = 0; i < ParameterLen; i++) // 将数据放入发送缓存
		{
			ChanData[(2 * i + 1)] = ((*Send1) >> 8) & 0xff;
			ChanData[(2 * i)] = ((*Send1++)) & 0xff;
		}
		write(Eth_Data.socket_descr, (&ChanData[0]), length); // 发送数据
	}
	// 发送校验位与信号尾
	num = 0;
	temp[num++] = SUM;
	temp[num++] = SUM >> 16;

	temp[num++] = 0x5C5A;
	temp[num++] = 0x5E5D;
	length = num * 2;
	for (i = 0; i < num; i++)
	{
		ChanLastData[(2 * i + 1)] = (temp[i] >> 8) & 0xff;
		ChanLastData[(2 * i)] = (temp[i]) & 0xff;
	}
	write(Eth_Data.socket_descr, (&ChanLastData[0]), length);
}
/******************************************************************
函数名称：EthernetReceiveFaiAGE
函数功能:数据接受失败标志，传输给上位机
输入参数：错误命令
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
static void EthernetReceiveFaiAGE(uint16_t Error)
{
	uint16_t temp[50], num = 0, i, length;
	uint8_t ChanDataFai[50];
	uint8_t IP_TEMP;
	uint32_t SUM;

	IP_TEMP = Read_IP;
	temp[num++] = 0xBBAA;
	temp[num++] = 0xDDCC;
	for (i = 0; i < 10; i++)
	{
		temp[num++] = (Eth_Data.Rec_Comand[(2 * i + 1 + 4)] << 8) | (Eth_Data.Rec_Comand[(2 * i + 4)]); // 去4个开头后是命令
	}
	temp[num++] = 0;

	temp[num++] = 0xC0A8;
	temp[num++] = (0x7B << 8) + IP_TEMP;

	temp[num++] = 0x1144;
	temp[num++] = 68;
	temp[num++] = 0;
	temp[num++] = Error; // 后面改成错误码
	SUM = Crs(temp, num);
	temp[num++] = SUM;
	temp[num++] = SUM >> 16;
	temp[num++] = 0x5C5A;
	temp[num++] = 0x5E5D;
	length = num * 2;
	for (i = 0; i < num; i++)
	{
		ChanDataFai[(2 * i + 1)] = (temp[i] >> 8) & 0xff;
		ChanDataFai[(2 * i)] = (temp[i]) & 0xff;
	}
	write(Eth_Data.socket_descr, (&ChanDataFai[0]), length);
}

/******************************************************************
函数名称：Crs
函数功能:校验和计算
输入参数：
输出参数：
返回值： 计算好的校验和
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
static uint32_t Crs(uint16_t *buf, uint16_t len)
{ // 校验和 = 命令 + 数据，len按字来算，而不是按字节算
	uint32_t i, sum = 0;
	uint16_t *ptr;

	if (len > 1536)
	{
		return 0;
	}
	ptr = buf;
	for (i = 0; i < len; i++)
	{
		sum += ((*(ptr + i)) & 0x00ff);
		sum += ((*(ptr + i)) >> 8);
	}

	return sum;
}
/******************************************************************
函数名称：
函数功能:
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
static void GetSoftWareVersion(void *parameter)
{
	uint16_t len;
	uint16_t SoftVerSend[20] = {0};

	len = sizeof(SoftVersionString); // 36
	if (len < 40)
	{
		memcpy((uint8_t *)SoftVerSend, &SoftVersionString, len);
	}
	else if (len > 40)
	{
		memcpy((uint8_t *)SoftVerSend, &SoftVersionString, 40);
	}
	EthernetReceiveSuccess(20, &SoftVerSend[0]);
}

/******************************************************************
函数名称：OnlineFlashProgram()
函数功能:程序在线下载
输入参数：
输出参数：
返回值：
注意：通过上位机直接下载程序给下位机，进行程序的更新
******************************************************************/
/******************************************************************
函数更改历史记录：第一步
******************************************************************/
static void OnlineFlashProgram(void *parameter)
{
	uint32_t ERROR;
	uint16_t jtemp, i;

	jtemp = (MAX_REC_COMSIZE / 2);
	Eth_Data.Ethernet.Flash_Legth.bytes.lbyte = Eth_Data.Ethernet.Data[jtemp++];
	Eth_Data.Ethernet.Flash_Legth.bytes.hbyte = Eth_Data.Ethernet.Data[jtemp++];

	memset(&App_Ctrl, 0, sizeof(struct APPDownloadCTRL)); // 清空标志位;

	App_Ctrl.MAX_Byte = Eth_Data.Ethernet.Flash_Legth.data;
	App_Ctrl.MAX_Num = App_Ctrl.MAX_Byte % 1024 ? App_Ctrl.MAX_Byte / 1024 + 1 : App_Ctrl.MAX_Byte / 1024;
	App_Ctrl.MAX_FlashWE = App_Ctrl.MAX_Num % 16 ? App_Ctrl.MAX_Num / 16 + 1 : App_Ctrl.MAX_Num / 16;
	App_Ctrl.Net_Buf = (uint32_t)&upgradeDATA[0];

	fmc_unlock();
	fmc_sector_erase(APP_Buff_Sector);//下载程序在此
	fmc_lock();

	EthernetReceiveSuccess(0, 0);
}
/******************************************************************
函数名称：ProgramUpdata
函数功能：将烧写程序放入ram中暂存
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：//第二步
******************************************************************/
static void ProgramUpdata(void *parameter)
{
	uint16_t jtemp;
	uint32_t i, ERROR;
	uint16_t order;
	volatile uint16_t j = 0;

	jtemp = (MAX_REC_COMSIZE / 2) - 3;
	App_Ctrl.Num = Eth_Data.Ethernet.Data[jtemp++];
	order = (App_Ctrl.Num - 1) % 16;
	jtemp = (MAX_REC_COMSIZE / 2);
	if (App_Ctrl.Num < App_Ctrl.MAX_Num)
		memcpy((uint32_t *)(App_Ctrl.Net_Buf), &Eth_Data.Ethernet.Data[jtemp], 1024);
	else
	{
		memcpy((uint32_t *)(App_Ctrl.Net_Buf), &Eth_Data.Ethernet.Data[jtemp], App_Ctrl.MAX_Byte % 1024);
		memset((uint32_t *)(App_Ctrl.Net_Buf+App_Ctrl.MAX_Byte % 1024), 0, (1024 - App_Ctrl.MAX_Byte % 1024));
	}
	taskENTER_CRITICAL();
  fmc_unlock();
  for(int i=0;i<1024/4;i++)//写入
  	fmc_word_program(APP_Buff_ADDRESS+App_Ctrl.Num*1024+i*4,((uint32_t*)(App_Ctrl.Net_Buf))[i]);
#ifdef Updata_Debug
  if(true==Check_Program(updata_program_add+updata_shift,buff_add,size))//校验
    SEGGER_RTT_printf(0,"write right!!\n");
	else
		SEGGER_RTT_printf(0,"write error!!\n");
  updata_shift+=size;
#endif
	fmc_lock();
	taskEXIT_CRITICAL();
	App_Ctrl.FlashBuff_Length +=1024;
	if (App_Ctrl.Num == App_Ctrl.MAX_Num || App_Ctrl.FlashBuff_Length > App_Ctrl.MAX_Byte)
	{
	taskENTER_CRITICAL();
  fmc_unlock();		
	fmc_sector_erase(Updata_Ctrl_Sector);//更新程序的Flash标志	
	
			App_Ctrl.CompeleteFlag = 1;
			for (uint8_t a = 0; a < sizeof(struct APPDownloadCTRL) / 4 + 1; a++)
			{
				fmc_word_program(Updata_Ctrl_ADDRESS+ 4 * a, ((uint32_t *)&App_Ctrl)[a]);
			}	
	fmc_lock();
	taskEXIT_CRITICAL();		
		/*MD5校验 或者CRC校验 在重启后进行*/
		rcu_all_reset_flag_clear();
			////			__disable_irq();
			////			__set_FAULTMASK(1);
			NVIC_SystemReset();
	}
	EthernetReceiveSuccess(0, 0);
	
}
/******************************************************************
函数名称：ProgramUpdata
函数功能：将烧写程序放入ram中暂存
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：//第二步
******************************************************************/
static void PC_DATAHandler(char *Sptr, uint16_t len)
{
	uint16_t i;
	char *Command = CMD_Assemble[0].CMD_String;
	char ptr[MAX_REC_COMSIZE];
	uint16_t CMD_Count = 0;

	for (i = 0; i < len; i++)
	{
		ptr[i] = *Sptr++;
	}
	for (i = 0; i < MAX_NUM_COM; i++)
	{
		if (String_Verify(Command, (char *)(ptr + 4)) == 1)
		{
			(*CMD_Assemble[CMD_Count].Run_Function)((void *)(0));
		}
		CMD_Count++;
		Command = CMD_Assemble[CMD_Count].CMD_String;
	}
}
/******************************************************************
函数名称：ProgramUpdata
函数功能：将烧写程序放入ram中暂存
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：//第二步
******************************************************************/
static uint8_t String_Verify(const char *String, char *a)
{
	uint16_t i = 0;
	for (; String[i]; i++)
	{
		if (String[i] != a[i])
		{
			return 0;
		}
	}
	if (a[i] != 0)
		return 0;
	return 1;
}
/******************************************************************
函数名称：
函数功能:
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void Delay_Us(uint32_t Time)
{
	for (uint16_t i = 0; i < Time; i++)
	{
		for (uint16_t j = 0; j < 10; j++)
			;
	}
}
/******************************************************************
函数名称：
函数功能:
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void Delay_Ms(uint32_t Delay)
{
	while (Delay--)
		Delay_Us(1000);
}

/********************************************************
 * @brief 控制风扇的命令
 * @param {void} *parameter
 * @return {void}
 * @warning
 **********************************************************/
static void FAN_RATECtrl(void *parameter)
{
}

/********************************************************
 * @brief 风扇开关
 * @param {void} *parameter
 * @return {void}
 * @warning
 **********************************************************/
static void FAN_CTRL(void *parameter)
{
}
/********************************************************
 * @brief 控报警灯
 * @param {void} *parameter
 * @return {void}
 * @warning
 **********************************************************/
static void ALE_LED_CTRL(void *parameter)
{
  uint16_t jtemp;
  uint8_t LIGHT_CHOSE;
  uint8_t BEEP_EN;
  uint8_t BEEP_SWITCH;
	uint8_t Alarm_CHOSE;

  jtemp = ((MAX_REC_LENGTH / 2));
  BEEP_EN = Eth_Data.Ethernet.Data[jtemp] >> 8;
	LIGHT_CHOSE = Eth_Data.Ethernet.Data[jtemp++];//低位在前
  BEEP_SWITCH = Eth_Data.Ethernet.Data[jtemp];
	Alarm_CHOSE = Eth_Data.Ethernet.Data[jtemp++]>>8;
  if (1== Alarm_CHOSE){
  if (1 == LIGHT_CHOSE)
  {
    if (1 == BEEP_SWITCH)
    {
      CtrlHC138_Y1(LED_ON, ALRED);
      CtrlHC138_Y1(LED_OFF, ALYELOW);
      CtrlHC138_Y1(LED_OFF, ALGREEN);
      CtrlHC138_Y1(LED_ON, ALBBEEP);
    }
    else
    {
      CtrlHC138_Y1(LED_ON, ALRED);
      CtrlHC138_Y1(LED_OFF, ALYELOW);
      CtrlHC138_Y1(LED_OFF, ALGREEN);
      CtrlHC138_Y1(LED_OFF, ALBBEEP);
    }
  }
  else if (3 == LIGHT_CHOSE)
  {
    CtrlHC138_Y1(LED_OFF, ALRED);
    CtrlHC138_Y1(LED_ON, ALYELOW);
    CtrlHC138_Y1(LED_OFF, ALGREEN);
    CtrlHC138_Y1(LED_OFF, ALBBEEP);
  }
  else if (2 == LIGHT_CHOSE)
  {
    CtrlHC138_Y1(LED_OFF, ALRED);
    CtrlHC138_Y1(LED_OFF, ALYELOW);
    CtrlHC138_Y1(LED_ON, ALGREEN);
    CtrlHC138_Y1(LED_OFF, ALBBEEP);
  }
}
	else  //二号灯，4567位
	{
  if (1 == LIGHT_CHOSE)
  {
    if (1 == BEEP_SWITCH)
    {
      CtrlHC138_Y1(LED_ON, ALRED+4);
      CtrlHC138_Y1(LED_OFF, ALYELOW+4);
      CtrlHC138_Y1(LED_OFF, ALGREEN+4);
      CtrlHC138_Y1(LED_ON, ALBBEEP+4);
    }
    else
    {
      CtrlHC138_Y1(LED_ON, ALRED+4);
      CtrlHC138_Y1(LED_OFF, ALYELOW+4);
      CtrlHC138_Y1(LED_OFF, ALGREEN+4);
      CtrlHC138_Y1(LED_OFF, ALBBEEP+4);
    }
  }
  else if (3 == LIGHT_CHOSE)
  {
    CtrlHC138_Y1(LED_OFF, ALRED+4);
    CtrlHC138_Y1(LED_ON, ALYELOW+4);
    CtrlHC138_Y1(LED_OFF, ALGREEN+4);
    CtrlHC138_Y1(LED_OFF, ALBBEEP+4);
  }
  else if (2 == LIGHT_CHOSE)
  {
    CtrlHC138_Y1(LED_OFF, ALRED+4);
    CtrlHC138_Y1(LED_OFF, ALYELOW+4);
    CtrlHC138_Y1(LED_ON, ALGREEN+4);
    CtrlHC138_Y1(LED_OFF, ALBBEEP+4);
  }		
	}

			EthernetReceiveSuccess(0, 0);
}

/********************************************************
 * @brief 切继电器
 * @param {void} *parameter
 * @return {void}
 * @warning
 **********************************************************/
static void RELAY_CTRL(void *parameter)
{
	
}

/********************************************************
 * @brief 上传数据
 * @param {void} *parameter
 * @return {void}
 * @warning
 **********************************************************/
static void SendData(void *parameter)
{
	if(1 == SysData.Aging_Pack)
    {
	   EthernetReceiveSuccess((sizeof(SysData.Aging_Pack_DataA) / 2), (uint16_t *)&SysData.Aging_Pack_DataA);
	}
	else if(0 == SysData.Aging_Pack)
	{
	   EthernetReceiveSuccess((sizeof(SysData.Aging_Pack_DataB) / 2), (uint16_t *)&SysData.Aging_Pack_DataB);
	}
}

/********************************************************
 * @brief 控设备灯
 * @param {void} *parameter
 * @return {void}
 * @warning
 **********************************************************/
static void DeviceLED(void *parameter)
{
	
}
/********************************************************
 * @brief
 * @param {void} *parameter
 * @return {void}
 * @warning
 **********************************************************/
static void Gate_State_Set(void *parameter)
{
}
/********************************************************
 * @brief
 * @param {void} *parameter
 * @return {void}
 * @warning
 **********************************************************/
static void Gate_Control(void *parameter)
{
	
}

/********************************************************
 * @brief
 * @param {void} *parameter
 * @return {void}
 * @warning
 **********************************************************/
static void DEBUG_PRINT(void *parameter)
{
	uint16_t Offset = 0, len;
	char *const p = (char *)malloc(1024);
	memset(p, 0, 1024); // 初始化
	len = Write2Mem(p, "asc=%d\n", 32);
	Offset += len;
	len = Write2Mem(p + Offset, "asc=%d", 32);
	Offset += len;
	EthernetReceiveSuccess(Offset / 2 ? Offset / 2 + 1 : Offset / 2, (uint16_t *)p);
	free(p);
}

/********************************************************
 * @brief
 * @param {void} *parameter
 * @return {void}
 * @warning
 **********************************************************/
static void GetData(void *parameter)
{
}
/********************************************************
 * @brief
 * @param {void} *parameter
 * @return {void}
 * @warning
 **********************************************************/
static void SetPower_ON(void *parameter)
{
}
/********************************************************
 * @brief
 * @param {void} *parameter
 * @return {void}
 * @warning
 **********************************************************/
static void SetPower_OFF(void *parameter)
{
}
/********************************************************
 * @brief
 * @param {void} *parameter
 * @return {void}
 * @warning
 **********************************************************/
static void SetPower_Reset(void *parameter)
{
}
/********************************************************
 * @brief
 * @param {void} *parameter
 * @return {void}
 * @warning
 **********************************************************/
static void SetPower_State(void *parameter)
{
}

/********************************************************
 * @brief
 * @param {void} *parameter
 * @return {void}
 * @warning
 **********************************************************/
static void SetFan_ON(void *parameter)
{
}
/********************************************************
 * @brief
 * @param {void} *parameter
 * @return {void}
 * @warning
 **********************************************************/
static void SetFan_OFF(void *parameter)
{
}
/********************************************************
 * @brief
 * @param {void} *parameter
 * @return {void}
 * @warning
 **********************************************************/
static void SetFan_Speed(void *parameter)
{
	uint16_t jtemp,Fan_Num,Fan_Speed,Set_Speed;
	

	jtemp = ((MAX_REC_LENGTH / 2));

	Fan_Num = Eth_Data.Ethernet.Data[jtemp++];
	Fan_Speed = Eth_Data.Ethernet.Data[jtemp++];

	Set_Speed=(uint16_t)((Fan_Speed/100.0)*2000);
	SysCtrl.SetFanSpeed=Set_Speed;
	
	FAN_SPEED_SET(Set_Speed);
    FAN_Start();

	EthernetReceiveSuccess(0, 0);
	
}

/********************************************************
 * @brief
 * @param {void} *parameter
 * @return {void}
 * @warning
 **********************************************************/
static void SetValve(void *parameter)
{
	uint16_t jtemp, i,ChanNo,SetData;

	jtemp = ((MAX_REC_LENGTH / 2));

	for (i = 0; i < AMC_DAC_NUM; i++)
	{
		ChanNo = i;
		SetData = Eth_Data.Ethernet.Data[jtemp++];
		AMC7836SetDAC(SetData, ChanNo);
	}

	EthernetReceiveSuccess(0, 0);
}

/********************************************************
 * @brief 在指定地址写入一串字符串，以类似Printf()函数格式的字符
 * @param {char *} pointer
 * @param {char*} Strings
 * @return {void}
 * @warning
 **********************************************************/
static uint16_t Write2Mem(char *const pointer, const char *Strings, ...)
{
	uint16_t counter = 0; // 计数
	va_list ap;			  // 定义
	// 错误处理
	if (pointer == (void *)0 || Strings == (void *)0)
		return 0;
	va_start(ap, Strings);
	while (*Strings != '\0')
	{
		if (*Strings == '%')
		{
			Strings++;
			switch (*Strings) // TODO:C++编译器和Keil 编译器不一定一样因此此处可能需要修改
			{
			case 'd': // 以整数形式打印
			{
				int num = va_arg(ap, int);
				unsigned int abs, remainder;
				char CNum[10];
				char *p;
				if (num < 0)
				{
					*(pointer + (counter)) = '-';
					counter++;
					abs = -num;
				}
				else
					abs = num;
				for (uint8_t j = 0; j < 10; j++)
				{
					remainder = abs % 10;
					CNum[9 - j] = remainder;
					abs /= 10;
				}
				p = CNum;
				while (*p == 0) // 跳过0
					p++;
				while (p < (CNum + 10))
				{
					*(pointer + (counter)) = *p + '0';
					p++;
					counter++;
				}
			}
			break;
			case 'u': // 以无符号正式形式打印
			{
				unsigned int num = (unsigned int)va_arg(ap, int), remainder;
				char CNum[10];
				char *p;

				for (uint8_t j = 0; j < 10; j++)
				{
					remainder = num % 10;
					CNum[9 - j] = remainder;
					num /= 10;
				}
				p = CNum;
				while (*p == 0) // 跳过0
					p++;

				while (p < (CNum + 10))
				{
					*(pointer + (counter)) = *p + '0';
					p++;
					counter++;
				}
			}
			break;
			case 's': // 输出字符串
			{
				const char *p;
				p = (const char *)va_arg(ap, int);
				while (*p != '\0')
				{
					*(pointer + (counter)) = *p;
					p++;
					counter++;
				}
			}
			break;
			case '%':
				*(pointer + (counter)) = '%';
				counter++;
				break;
			default:
				break;
			}
			Strings++;
		}
		else
		{
			*(pointer + (counter)) = *Strings;
			Strings++;
			counter++;
		}
	}
	va_end(ap);
	return counter;
}
/**************************************************************************
函数名称:MMBSDCtrl(void *parameter)
函数功能:接收监控主控板的控制
输入参数:
输出参数:
返回值　:
注　　意:
完成时间:2022年03月02日15:50:34
函数更改历史记录:
更改时间:
**************************************************************************/
void WriteKB(void *parameter)
{
	uint16_t i, j, jtemp, Cali_Obj;
	uint32_t ERROR;
	
	jtemp = (MAX_REC_COMSIZE / 2);
	KB_TYPE kb[Cail_Num] = {0};
	Cali_Obj = Eth_Data.Ethernet.Data[jtemp++];
	
	
	if (Cali_Obj != 1)
		return;
	for (uint8_t it = 0; it < Cail_Num; it++)
	{
		kb[it].KVal.bytes.lbyte = Eth_Data.Ethernet.Data[jtemp++];
		kb[it].KVal.bytes.hbyte = Eth_Data.Ethernet.Data[jtemp++];
		kb[it].BVal.bytes.lbyte = Eth_Data.Ethernet.Data[jtemp++];
		kb[it].BVal.bytes.hbyte = Eth_Data.Ethernet.Data[jtemp++];
	}
	// 写KB
	fmc_unlock();
	fmc_sector_erase(KB_Sector);//

	for (j = 0; j < sizeof(kb) / 4; j++) // 写BK值 1000倍
	{
		fmc_word_program(KB_ADDR + 4 * j, *((uint32_t *)(((uint32_t)&kb) + 4 * j)));
	}
	fmc_lock();
	EthernetReceiveSuccess(0, 0);
}
/**************************************************************************
函数名称:MMBSDCtrl(void *parameter)
函数功能:接收监控主控板的控制
输入参数:
输出参数:
返回值　:
注　　意:
完成时间:2022年03月02日15:50:34
函数更改历史记录:
更改时间:
**************************************************************************/
void SendKB(void *parameter)
{
	KB_TYPE *kb = (KB_TYPE *)KB_ADDR;
	uint16_t data1[Cail_Num * 4 + 1];
	data1[0] = 1;
	uint16_t(*data)[4] = (uint16_t(*)[4]) & data1[1];
	for (uint8_t i = 0; i < Cail_Num; i++)
	{
		data[i][0] = kb->KVal.bytes.lbyte;
		data[i][1] = kb->KVal.bytes.hbyte;
		data[i][2] = kb->BVal.bytes.lbyte;
		data[i][3] = kb->BVal.bytes.hbyte;
		kb++;
	}
	EthernetReceiveSuccess(1 + 4 * Cail_Num, (uint16_t *)data1);
}
