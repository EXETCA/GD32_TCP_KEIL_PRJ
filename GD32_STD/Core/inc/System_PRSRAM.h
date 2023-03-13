#ifndef _SYSTEM_PRSRAM_H_
#define _SYSTEM_PRSRAM_H_

/**************************************************************************
                                 INCLUDES
***************************************************************************/
#define PROGPC_HEAD_BYTES 256
#define PROGPC_HEAD_WORDS 128 //除通道号外前面公用部分

#define STEP_HEAD_BYTES 2
#define STEP_HEAD_WORDS 1
#define STEP_END_CONDITION_BYTES 2
#define STEP_END_CONDITION_WORDS 1

#define SINGLE_PACKAGE_BYTES 600 //(SINGLE_SECTION_BYTES * MAX_SECTION_COUNT + 6)
#define SINGLE_PACKAGE_WORDS  300//(SINGLE_SECTION_WORDS * MAX_SECTION_COUNT + 3)

#define SAVE_BUFFA_SIZE 600
#define SAVE_BUFFB_SIZE 600

/*网络协议相关宏定义在tcpecho.h中定义*/
#endif
