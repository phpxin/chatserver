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

int pkg_parse(void *pkg);
int state_machine(int state, void *data);
