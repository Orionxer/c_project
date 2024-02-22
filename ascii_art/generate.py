import pyfiglet

# 选择ASCII艺术字体风格
font = "banner3"
# 需要生成的内容
text = "RTOS"
# 是否需要支持ANSI转义序列
enable_color = 0
# 选择输出颜色
color = "yellow"
# ANSI颜色映射表(可以根据ANSI规范自行)
ansi_map = {
    "red"       : "\x1b[31m",
    "green"     : "\x1b[32m",
    "yellow"    : "\x1b[33m",
    "blue"      : "\x1b[34m",
    "magenta"   : "\x1b[35m",
    "cyan"      : "\x1b[36m",
    "reset"     : "\x1b[0m"
}
# 目标字符串
result = pyfiglet.figlet_format(text, font=font)
# 如果启用了颜色输出则加入ANSI颜色映射
if enable_color:
    result = ansi_map[color] + result + ansi_map["reset"]
# 测试打印效果
print(result)

# 禁止转义
result = repr(result)
# 删除单引号'
result = result.replace("'", "")

# 生成C代码内容
c_code = """
#include <stdio.h>
// 目标字符串
const char *str = "%s";
int main() {
    printf("%%s\\n", str);
    return 0;
}
"""

# 将目标字符串连接到final_c_code中
final_c_code = c_code % result
# 保存文件
with open("ascii.c", "w") as file:
    file.write(final_c_code)

print("C source file saved!")
