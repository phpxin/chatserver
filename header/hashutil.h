/**
 * 查找配置值
 */
gboolean chat_config_search_call(gpointer key, gpointer value, gpointer user_data);

/**
 * 查找客户端连接
 */
gboolean chat_cinfo_search_call(gpointer key, gpointer value, gpointer user_data);
gboolean chat_cinfo_search_withfd(gpointer key, gpointer value, gpointer user_data);
