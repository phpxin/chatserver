/* define data structs */
struct user{
	int id;
	char account[200];
	char pwd[200];
	char name[200];
};

struct message{
	int id;
	int uid;
	int fid;
	char content[65535];
	short type;
};

/*
 * 连接数据库
 * @return int 0/1 失败/成功
 */
int init_db();

/*
 * 关闭数据库
 * @return int 0/1 失败/成功
 */
int close_db();

/**
 * 登录用户获取信息
 * @param const char *account 用户账号
 * @param const char *pwd 用户密码
 * @param struct user *_u 返回值指针
 *
 * @return struct user 用户结构
 * -1 登录账号错误
 * -2 密码错误
 * -3 数据库错误
 * 0 成功（将返回指针设为查到的用户）
 * 获取登录用户信息
 */
int get_users(const char *where, struct user **users, size_t *ucount);

/**
 * 通过id获取用户信息
 *
 * @param int id 用户id
 * @param struct user *_u 返回值指针
 *
 * @return -3 数据库错误 -2 未找到 0 成功
 *
 */
int get_user(int id, struct user *_u);

/**
 * 插入聊天数据
 * @param struct message *msg 消息正文结构体
 * @return -3 数据库错误；0 插入0条；返回插入条数
 */
int insert_message(struct message *msg);
