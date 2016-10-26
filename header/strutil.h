/**
 * su_trim
 * 去除两边的指定字符
 * return 1/0 成功/失败
 */
int su_trim(char *str, char *cs);

/**
 * su_rtrim
 * 去除右边的指定字符
 * return 1/0 成功/失败
 */
int su_rtrim(char *str, char *cs);

/**
 * su_ltrim
 * 去除左边的指定字符
 * return 1/0 成功/失败
 */
int su_ltrim(char *str, char *cs);

/**
 * su_cfpos
 * 查找某字符首次出现的位置
 * return -1 未找到，0-n 为字符c在字符串中出现的位置
 */
int su_cfpos(char *arr, char c);

/**
 * su_explode
 * 字符串切分
 * char c 切分字符
 * char * in 输入的字符串
 * char * out 输出的字符串数组，会以NULL结尾
 * int * out_len 输出的字符串数组的长度
 * return 1/0 成功/失败
 */
int su_explode(char c, const char *in, char *out[], int *out_len) ;

/**
 * free_string_array
 * 释放字符串数组内存
 * char *str_array[] 字符串数组，以NULL结尾
 * int len 数组长度
 * return 1/0 成功/失败
 */
int free_string_array(char *str_array[], int len) ;