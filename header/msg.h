
/**
 * 消息处理主函数
 * @return int 1 on success , 0 on failed
 */
int msg(int fd);

/**
 * 读取数据
 * @return int 1 on success , 0 on failed
 * @info 去掉协议中的长度字段
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
 * @info pkg为已去掉协议字段的数据包
 */
RET protocol_stat_machine(unsigned short protocol, const void *pkg, size_t pkg_len);

/**
 * 返回消息
 * @param const void *udata 用户数据
 */
void response(int fd, int flag, unsigned short protocol, const void *udata, size_t udata_len);
