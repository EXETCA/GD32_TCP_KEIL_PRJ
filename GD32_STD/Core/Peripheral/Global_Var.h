#include "Myinclude.h"
struct SYS_CTRL SysCtrl;
struct SysData SysData;
volatile uint8_t EN_DBUS_DATA = 0, CS_DBUS_DATA = 0, PWR_CTLBUS_DATA = 0, DATA_DBUS_DATA = 0, IO_CTLBUS_DATA = 0;
uint8_t BordState = {0};
uint8_t Samp_Counter = 0;
uint16_t num = 0, num1 = 0;
uint8_t Fan_Speed = 0;
uint32_t Cur_Frist[16] = {0};
uint32_t NorCur;
uint8_t NorCurCnt;
uint8_t Re_Buff[30];

volatile uint8_t ReciveFlag = 0;
uint32_t Capture_TIM[8][2] = {0};
volatile uint8_t Capture_Cnt[8] = {0};
