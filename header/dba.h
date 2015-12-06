#include <time.h>

/* define data structs */
#define CHAR_SIZE_1 200
#define TEXT_SIZE_1 65535

struct user{
	int id;
	char account[CHAR_SIZE_1];
	char pwd[CHAR_SIZE_1];
	char name[CHAR_SIZE_1];
	char avatar[CHAR_SIZE_1];
};

struct message{
	int id;
	int uid;
	int fid;
	char content[TEXT_SIZE_1];
	short type;
    time_t addtime;
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
 * @param const char *where 条件
 * @param struct user **users 用户列表
 * @param size_t *ucount 用户数（列表长度）
 * @return int 0/1 失败/成功; -xxx 错误
 */
int get_users(const char *where, struct user **users, size_t *ucount);

/**
 * 通过id获取用户信息
 * @param int id 用户id
 * @param struct user *_u 返回值指针
 * @return int 0/1 失败/成功; -xxx 错误，根据DBE_xx常量获取; 
 */
int get_user(int id, struct user *_u);

/**
 * 插入聊天数据
 * @param struct message *msg 消息正文结构体
 * @return int 插入条数/-xxx 错误
 */
int insert_message(struct message *msg);
