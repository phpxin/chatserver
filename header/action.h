/**
 * 所有action程序
 * @return 成功返回 SUCC , 失败返回 FAILED , 错误返回 ERR 并设置 exception_no
 */
STATUS act_user_login(const void *pkg, size_t pkg_len);
STATUS act_user_message(const void *pkg, size_t pkg_len);
