#include <stdio.h>
#include <stdbool.h>
#include "debug_log.h"
#include <signal.h>
#include <unistd.h>

/**********************************************************************************************
 * @brief   状态机四要素：[现态]，[条件]，[动作]以及[次态]。
 *          通过当前状态机实例的[现态]和[条件]查找状态表，如果找到匹配的[现态]和[条件]，
 *          则保存状态表的动作(回调函数)以及次态。状态机实例执行[动作]，切换[状态](现态切换为次态)，
 *          并等待[条件]更新，如果[条件]没有更新则循环执行当前[动作]；如果条件更新则继续查找状态表，
 *          重复以上的逻辑
 * @note    Finite State Machine    有限状态机
 * @param   state       现态
 * @param   event       条件
 * @param   action      动作(回调函数，根据实际情况决定是否需要带参数)
 * @param   next_state  次态
***********************************************************************************************/

// 项目名称
#define PROJECT_NAME "\x1b[33m ____ ____ ____ \n||F |||S |||M ||\n||__|||__|||__||\n|/__\\|/__\\|/__\\|\n\x1b[0m"
// 电梯底层
#define MIN_FLOOR   -3
// 电梯顶层
#define MAX_FLOOR   20
// 电梯运行一层楼梯所需要的时间(单位秒)
#define RUN_TIME    1

// 计算数组长度
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

// 电梯状态
typedef enum
{
    STATE_IDLE,
    STATE_GOING_UP,
    STATE_GOING_DOWN,
}en_state_t;

// 电梯事件
typedef enum
{
    EVENT_UP,
    EVENT_DOWN,
    EVENT_ARRIVE
}en_event_t;

// 状态机
typedef struct
{
    // FSM成员变量
    en_state_t state;       // 现态
    en_event_t event;       // 条件
    void (*action)(int *);  // 动作
    en_state_t next_state;  // 次态
}stc_fsm_t;

// 上升动作
void go_up_action(int *floor)
{
    DBG_LOGI("\t🔼 Going UP, current_floor = %d", *floor);
    // 启动定时器(启动电梯)
    alarm(RUN_TIME);
    (*floor)++;
}

// 下降动作
void go_down_action(int *floor)
{
    DBG_LOGI("\t🔽 Going DOWN, current_floor = %d", *floor);
    // 启动定时器(启动电梯)
    alarm(RUN_TIME);
    (*floor)--;
}

// 到达指定楼层动作
void arrive_action(int *floor)
{
    DBG_LOGI("\t⏸  === Arrived target floor: %d", *floor);
}

// 状态表
stc_fsm_t fsm_map[] = 
{
    {STATE_IDLE,        EVENT_UP,           go_up_action,   STATE_GOING_UP},
    {STATE_GOING_UP,    EVENT_ARRIVE,       arrive_action,  STATE_IDLE},
    {STATE_IDLE,        EVENT_DOWN,         go_down_action, STATE_GOING_DOWN},
    {STATE_GOING_DOWN,  EVENT_ARRIVE,       arrive_action,  STATE_IDLE},
};

/**
 * @brief   根据状态机 [现态] 以及 [条件] 尝试查询并保存 [次态] 以及 [动作]
 * @param   [in/out] fsm    实时状态机指针
 * @return  是否找到相同的现态以及条件 
*/
bool traverse_fsm_map(stc_fsm_t *fsm)
{
    // 遍历状态表
    for (size_t i = 0; i < ARRAY_SIZE(fsm_map); i++)
    {
        // [现态] 以及 [条件] 均满足
        if (fsm->state == fsm_map[i].state && fsm->event == fsm_map[i].event)
        {
            // 保存次态
            fsm->next_state = fsm_map[i].next_state;
            // 保存动作(保存回调函数)
            fsm->action = fsm_map[i].action;
            // 找到返回true, 结束循环
            return true;
        }
    }
    // 没有找到则返回false
    return false;
}


// 初始化状态机
stc_fsm_t fsm = {0};
// 当前楼层
int current_floor = 0;
// 目标楼层
int target_floor = 0;

/**
 * @brief   定时器回调函数
 * @note    用于模拟电梯运行时需要的时间
*/
void timer_callback(int signum)
{
    if (fsm.state != STATE_IDLE) // 运行中
    {
        if (current_floor == target_floor) // 到达指定楼层
        {
            fsm.event = EVENT_ARRIVE;
        }
        // 查询状态表
        if (traverse_fsm_map(&fsm))
        {
            // 执行动作
            fsm.action(&current_floor);
            // 切换状态
            fsm.state = fsm.next_state;
        }
        else // 如果找不到新的状态则循环执行当前动作
        {
            fsm.action(&current_floor);
        }
    }
    if (fsm.state == STATE_IDLE)
    {
        DBG_LOGI("Waiting for new target floor: ");
    }
}

int main(void)
{
    printf("%s\n", PROJECT_NAME);
    DBG_LOGI("The Elevator floor range : [%d] to [%d]", MIN_FLOOR, MAX_FLOOR);
    DBG_LOGI("Current_floor = %d", current_floor);
    DBG_LOGI("Waiting for new target floor: ");
    // 设置定时器回调处理函数
    signal(SIGALRM, timer_callback);
    int floor = 0;
    while (1)
    {
        scanf("%d", &floor);
        // 电梯判忙
        if (fsm.state != STATE_IDLE)
        {
            DBG_LOGW("Elevator is running, ignore this command");
            continue;
        }
        // 值域判断
        if (floor < MIN_FLOOR || floor > MAX_FLOOR)
        {
            DBG_LOGW(" Target Floor [%d] out of range", floor);
            continue;
        }
        // 同一楼层
        if (floor == current_floor)
        {
            DBG_LOGI("You already in floor %d, elevator will not run", floor);
            continue;
        }
        target_floor = floor;
        DBG_LOGI("######## Target floor is %d", target_floor);
        if (target_floor > current_floor)
        {
            fsm.event = EVENT_UP;
        }
        if (target_floor < current_floor)
        {
            fsm.event = EVENT_DOWN;
        }
        // 查询状态表
        if (traverse_fsm_map(&fsm))
        {
            // 执行动作
            fsm.action(&current_floor);
            // 切换状态
            fsm.state = fsm.next_state;
        }
        else
        {
            DBG_LOGW("Not found valid fsm");
        }
    }
    return 0;
}