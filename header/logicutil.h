
/**
 * 记录/显示日志
 */
void elog(const char * , ...);

/**
 * 释放变量
 */
void myfree(void *var);

/**
 * 主机字节序大小端判断
 */
int blOrll();

/**
 * int转换成网络字节序
 */
int int_to_net(int value);

/**
 * 网络字节序转换int
 */
int net_to_int(int value);

/**
 * 大小端相互转换
 */
int big_litle_endian(int x);
