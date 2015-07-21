#define PTO_LOGIN 1001	/* protocol login */
#define PTO_MSG 1002	/* protocol message */

/**
 * 消息处理主函数
 * @return int 1 on success , 0 on failed
 */
int msg(int fd);

/**
 * 读取数据
 * @return int 1 on success , 0 on failed
 */
int msg_read(int fd, void **pkg, size_t *pkg_len);

/**
 * 发送数据
 * @return int 1 on success , 0 on failed
 */
int msg_write(int fd, void *pkg, size_t pkg_len);

/**
 * 协议状态机
 * @param unsigned short protocol 协议
 * @param const void *pkg 数据包
 * @param size_t 数据包大小
 */
void protocol_stat_machine(unsigned short protocol, const void *pkg, size_t pkg_len);
