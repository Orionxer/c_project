#ifndef DEBUG_LOG_H_
#define DEBUG_LOG_H_
#include <stdint.h>
#include <string.h>

/**
 * @brief   尽可能确保显示的终端支持ANSI的转移序列以及UTF-8的显示
 * @note    当终端不支持ANSI的转移序列，则无法正常显示颜色信息
 * @note    当终端不支持UTF-8，则关于中文的显示会乱码
*/

/************************** 用户自定义宏 *********************************/
/**
 * @brief   调试输出总开关
 * @note    处于禁用状态时关闭所有输出
*/
#define DGB_ENABLE          1

/**
 * @brief   指定 输出打印等级
 * @note    1. 只会打印当前等级及其以上的等级的信息
 * @note    2. 假设 指定的打印等级 = DGB_LOG_WARNING,
 *          则只会打印 DGB_LOG_WARNING 和 DGB_LOG_ERROR两个等级的信息
*/
#define DBG_LOG_LEVEL       DGB_LOG_DEBUG


/*************************** 调试输出保留宏 ********************************/
/**
 * @brief   输出的等级从高到低的排序: [错误] > [警告] > [普通] > [调试]
*/
// [错误]输出等级 
#define DGB_LOG_ERROR       1
// [警告]输出等级
#define DGB_LOG_WARNING     2
// [普通]输出等级
#define DGB_LOG_INFO        3
// [调试]输出等级
#define DGB_LOG_DEBUG       4

#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_BLUE     "\x1b[34m"
#define ANSI_COLOR_MAGENTA  "\x1b[35m"
#define ANSI_COLOR_CYAN     "\x1b[36m"
#define ANSI_COLOR_RESET    "\x1b[0m"

// 只获取文件名
#define filename(x) strrchr(x,'/')?strrchr(x,'/')+1:x

// 调试输出总开关 处于打开状态
#if DGB_ENABLE
// [调试]等级控制
#if DBG_LOG_LEVEL >= DGB_LOG_DEBUG
#define DBG_LOGD(...) \
    printf(ANSI_COLOR_BLUE); \
    printf("[%s]: ", __func__); \
    printf(__VA_ARGS__); \
    printf(ANSI_COLOR_RESET); \
    printf("\n");
#else
#define DBG_LOGD(...)
#endif

// [普通]等级控制
#if DBG_LOG_LEVEL >= DGB_LOG_INFO
#define DBG_LOGI(...) \
    printf(ANSI_COLOR_GREEN); \
    printf("[%s]: ", __func__); \
    printf(__VA_ARGS__); \
    printf(ANSI_COLOR_RESET); \
    printf("\n");
#else
#define DBG_LOGI(...)
#endif

// [警告]等级控制
#if DBG_LOG_LEVEL >= DGB_LOG_WARNING
#define DBG_LOGW(...) \
    printf(ANSI_COLOR_YELLOW); \
    printf("[%s:%d %s]: ", filename(__FILE__), __LINE__, __func__); \
    printf(__VA_ARGS__); \
    printf(ANSI_COLOR_RESET); \
    printf("\n");
#else
#define DBG_LOGW(...)
#endif

// [错误]等级控制
#if DBG_LOG_LEVEL >= DGB_LOG_ERROR
#define DBG_LOGE(...) \
    printf(ANSI_COLOR_RED); \
    printf("[%s:%d %s]: ", filename(__FILE__), __LINE__, __func__); \
    printf(__VA_ARGS__); \
    printf(ANSI_COLOR_RESET); \
    printf("\n");
#else
#define DBG_LOGE(...)
#endif
#else
#define DBG_LOGD(...)
#define DBG_LOGI(...)
#define DBG_LOGW(...)
#define DBG_LOGE(...)
#endif
/**
 * @brief   以16进制打印数据
 * @param   [in] data   打印的数据数据
 * @param   [in] len    数据长度 
 * @note    输出格式 = %02X
*/
void print_hex_table(uint8_t *data, uint16_t len);

/**
 * @brief   彩色打印示例
 * @note    打印各个等级的信息以及16进制的格式化输出
*/
void debug_log_demo(void);

#endif
