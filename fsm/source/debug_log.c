#include <stdio.h>
#include "debug_log.h"


void print_hex_table(uint8_t *data, uint16_t len)
{
#if DBG_ENABLE
    // 打印表头
    printf("     ");
    PRINT_ANSI_COLOR(ANSI_COLOR_MAGENTA);
    for (size_t i = 0; i < 0x10; i++)
    {
        printf("%02X ", (unsigned int)i);
    }
    PRINT_ANSI_COLOR(ANSI_COLOR_RESET);
    printf("\n");
    // 打印表列以及有效数据, 每打印16个数据换一行
    for (size_t i = 0; i < len; i++)
    {
        if (i % 16 == 0)
        {
            PRINT_ANSI_COLOR(ANSI_COLOR_MAGENTA);
            printf("%04X ", (unsigned int)i);
            PRINT_ANSI_COLOR(ANSI_COLOR_RESET);
        }
        PRINT_ANSI_COLOR(ANSI_COLOR_CYAN);
        printf("%02X ", data[i]);
        PRINT_ANSI_COLOR(ANSI_COLOR_RESET);
        if ((i + 1) % 16 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
#endif
}

void debug_log_demo(void)
{
    // 定义项目名称
    #define PROJECT_NAME_ART        " ____ ____ ____ ____ ____ ____ ____ ____ \n||L |||O |||G |||- |||D |||E |||M |||O ||\n||__|||__|||__|||__|||__|||__|||__|||__||\n|/__\\|/__\\|/__\\|/__\\|/__\\|/__\\|/__\\|/__\\|\n"
    // 彩色打印Demo
    ADVANCED_LOG(ANSI_COLOR_YELLOW, PROJECT_NAME_ART);
    DBG_LOGI("中文测试输出, 需要确保终端支持UTF-8的显示, 否则显示乱码\n");
    // 输出所有等级测试，可以尝试修改打印等级的宏以观察输出效果 DBG_LOG_LEVEL
    DBG_LOGD("The Log Level is Debug");
    DBG_LOGI("The Log Level is Info");
    DBG_LOGW("The Log Level is Warning");
    DBG_LOGE("The Log Level is Error\n");
    // 打印16进制
    uint8_t array[100] = {0};
    for (size_t i = 0; i < 100; i++)
    {
        array[i] = i;
    }
    print_hex_table(array, 100);
#if DBG_ENABLE == 0
    printf("## Debug Print Disable, NO log print\n");
#endif
}
