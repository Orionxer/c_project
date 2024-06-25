#include <stdio.h>
#include "debug_log.h"

#define STRING_DATA 1
#define BYTE_ARRAY  2

#define RAW_DATA_TYPE   BYTE_ARRAY

/**
 * @brief   Base64终端测试命令
 * ****************** Base64 编码(字节数组) **************************
 * @test    byte_array=(0x02, 0x01, 0x05, 0x03, 0x03, 0x54, 0x56, 0x0F, 0x09, 0x56, 0x69, 0x53, 0x4E, 0x5F, 0x53, 0x4C)
 * @test    echo -n "${byte_array[@]}" | xxd -r -p | base64
 * @result  AgEFAwNUVg8JVmlTTl9TTA==
 * 
 * ****************** Base64 编码(字符串) **************************
 * @test    echo -n "Hello World" | base64
 * @result  SGVsbG8gV29ybGQ=
 * 
 * ****************** Base64 解码(16个数据换行) **************************
 * @test    echo "AgEFAwNUVg8JVmlTTl9TTA==" | base64 --decode | xxd -u -g 1
 * @test    echo "AgEFAwNUVg8JVmlTTl9TTA==" | base64 --decode | xxd -u -g 1 | sed 's/\([0-9A-F][0-9A-F]\)/\x1b[1;33m\1\x1b[0m/g'
 * @result  结果输出： 00000000: 02 01 05 03 03 54 56 0F 09 56 69 53 4E 5F 53 4C  .....TV..ViSN_SL
 * 
 * @test    echo "SGVsbG8gV29ybGQ=" | base64 --decode | xxd -u -g 1
 * @test    echo "SGVsbG8gV29ybGQ=" | base64 --decode | xxd -u -g 1 | GREP_COLOR='1;33' grep --color=always -E '[0-9A-F]{2}'
 * @result  结果输出： 00000000: 48 65 6C 6C 6F 20 57 6F 72 6C 64                 Hello World
 */

// Base64字符集
static const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Base64解码表
static const unsigned char base64_table[256] = {
    ['A'] = 0,  ['B'] = 1,  ['C'] = 2,  ['D'] = 3,  ['E'] = 4,  ['F'] = 5,  ['G'] = 6,  ['H'] = 7,
    ['I'] = 8,  ['J'] = 9,  ['K'] = 10, ['L'] = 11, ['M'] = 12, ['N'] = 13, ['O'] = 14, ['P'] = 15,
    ['Q'] = 16, ['R'] = 17, ['S'] = 18, ['T'] = 19, ['U'] = 20, ['V'] = 21, ['W'] = 22, ['X'] = 23,
    ['Y'] = 24, ['Z'] = 25, ['a'] = 26, ['b'] = 27, ['c'] = 28, ['d'] = 29, ['e'] = 30, ['f'] = 31,
    ['g'] = 32, ['h'] = 33, ['i'] = 34, ['j'] = 35, ['k'] = 36, ['l'] = 37, ['m'] = 38, ['n'] = 39,
    ['o'] = 40, ['p'] = 41, ['q'] = 42, ['r'] = 43, ['s'] = 44, ['t'] = 45, ['u'] = 46, ['v'] = 47,
    ['w'] = 48, ['x'] = 49, ['y'] = 50, ['z'] = 51, ['0'] = 52, ['1'] = 53, ['2'] = 54, ['3'] = 55,
    ['4'] = 56, ['5'] = 57, ['6'] = 58, ['7'] = 59, ['8'] = 60, ['9'] = 61, ['+'] = 62, ['/'] = 63
};

// Base64编码函数
void base64_encode(const unsigned char *input, size_t len, char *output) {
    char *p = output;
    for (size_t i = 0; i < len; i += 3) {
        unsigned int v = input[i] << 16;
        v |= (i + 1 < len ? input[i + 1] << 8 : 0);
        v |= (i + 2 < len ? input[i + 2] : 0);

        *p++ = base64_chars[(v >> 18) & 0x3F];
        *p++ = base64_chars[(v >> 12) & 0x3F];
        *p++ = (i + 1 < len ? base64_chars[(v >> 6) & 0x3F] : '=');
        *p++ = (i + 2 < len ? base64_chars[v & 0x3F] : '=');
    }
    *p = '\0';
}

// Base64解码函数
int base64_decode(const char *input, unsigned char *output, int *out_len) {
    size_t len = strlen(input);
    if (len % 4 != 0) return -1; // 无效的Base64输入

    size_t decoded_len = len / 4 * 3;
    if (input[len - 1] == '=') decoded_len--;
    if (input[len - 2] == '=') decoded_len--;

    unsigned char *p = output;
    for (size_t i = 0; i < len; i += 4) {
        unsigned int v = base64_table[(unsigned char)input[i]] << 18;
        v |= base64_table[(unsigned char)input[i + 1]] << 12;
        v |= input[i + 2] == '=' ? 0 : base64_table[(unsigned char)input[i + 2]] << 6;
        v |= input[i + 3] == '=' ? 0 : base64_table[(unsigned char)input[i + 3]];

        *p++ = (v >> 16) & 0xFF;
        if (input[i + 2] != '=') *p++ = (v >> 8) & 0xFF;
        if (input[i + 3] != '=') *p++ = v & 0xFF;
    }

    *out_len = p - output;
    return 0;
}


int main(void)
{
    // 彩色打印Demo
    // debug_log_demo();
// 字符串 编解码测试
#if RAW_DATA_TYPE == STRING_DATA
    const unsigned char *text = (unsigned char *)"Hello, World!";
    char encoded[256];
    unsigned char decoded[256];
    // ** 传入实际字符串的长度
    base64_encode(text, strlen((const char *)text), encoded);
    DBG_LOGI("Encoded: %s, len= %d", encoded, (int)strlen(encoded));
    int decoded_len = 0;
    if (base64_decode(encoded, decoded, &decoded_len) == 0)
    {
        DBG_LOGI("Decoded String: %.*s", (int)decoded_len, decoded);
    }
    else
    {
        DBG_LOGI("Invalid Base64 input");
    }
#endif

// 字节数组 编解码测试
#if RAW_DATA_TYPE == BYTE_ARRAY
    const uint8_t text[] = {0x02, 0x01, 0x05, 0x03, 0x03, 0x54, 0x56, 0x0F, 0x09, 0x56, 0x69, 0x53, 0x4E, 0x5F, 0x53, 0x4C};
    char encoded[256];
    unsigned char decoded[256];
    // ** 传入实际字节数组的长度
    base64_encode(text, sizeof(text), encoded);
    DBG_LOGI("Encoded: %s, len= %d", encoded, (int)strlen(encoded));

    int decoded_len = 0;
    if (base64_decode(encoded, decoded, &decoded_len) == 0)
    {
        DBG_LOGI("Decoded Byte Array:  len= %d", decoded_len);
        print_hex_table(decoded, decoded_len);
    }
    else
    {
        DBG_LOGI("Invalid Base64 input");
    }
#endif
    return 0;
}