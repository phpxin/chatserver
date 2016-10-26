typedef struct _AuthCode{

	int uid ;
	int unixtime ;

} AuthCode ;

/**
 * 解密authcode
 * @param unsigned char *in 加密的authcode,加密数据采用base64(aes_128_cbc(data))
 * @return AuthCode * 
 */
AuthCode* authcode_de(unsigned char *in);

/**
 * 解密/加密数据
 * @param unsigned char *in 输入加密/明文字符串，加密数据采用aes_128_cbc(data)
 * @param unsigned char **out 输出解密/加密字符串
 * @param int do_encrypt 1 加密，0 解密， -1 leave the value unchanged
 * @return int 1 on success , 0 on failed
 * @info 解密/加密数据
 */
int do_crypt2(unsigned char *in, unsigned char **out, int do_encrypt) ;