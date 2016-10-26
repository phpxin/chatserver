#include "apue.h"
#include "stdarg.h"
#include "config.h"
#include "logicutil.h"
#include "strutil.h"

#include <b64/base64.h>
#include <openssl/ssl.h>
#include <crypt.h>

AuthCode* authcode_de(unsigned char *in)
{
    char *dep = base64_decode(in);

    
    char *decode = (char *)malloc(BUFSIZE);
    memset(decode, '\0', BUFSIZE);

    int flag = do_crypt2(dep, &decode, 0);

    if (flag)
    {
        /* code */
        char *arr[] = {} ;
        int arr_len = 0 ;
        su_explode('_', decode, arr, &arr_len) ;

        if (arr_len < 2 )
        {
            /* free memory */
            if(arr_len>0) free_string_array(arr, arr_len) ;

            elog(E_ERROR, "read authcode failed src data is %s", decode);
            return NULL ;
        }

        AuthCode *ac = (AuthCode *)malloc(1) ;
        ac->uid = atoi(arr[0]) ;
        ac->unixtime = atoi(arr[1]) ;

        /* free memory */
        free_string_array(arr, arr_len) ;

        return ac ;
    }

    elog(E_ERROR, "decrypt authcode failed src data is %s", in);
    return NULL ;
}

int do_crypt2(unsigned char *in, unsigned char **out, int do_encrypt)
{
    /* Allow enough space in output buffer for additional block */
    unsigned char inbuf[1024], outbuf[1024 + EVP_MAX_BLOCK_LENGTH];
    int inlen, outlen;
    EVP_CIPHER_CTX ctx;
    /* Bogus key and IV: we'd normally set these from
     * another source.
     */
    unsigned char key[] = "0123456789abcdeF";
    unsigned char iv[]  = "1234567887654321";

    /* Don't set key or IV right away; we want to check lengths */
    EVP_CIPHER_CTX_init(&ctx);
    /****
    参数 do_encrypt：
    1. 加密
    0. 解密
    -1 leave the value unchanged (the actual value of 'enc' being supplied in a previous call)
    ***/
    EVP_CipherInit_ex(&ctx, EVP_aes_128_cbc(), NULL, NULL, NULL,
            do_encrypt);
    OPENSSL_assert(EVP_CIPHER_CTX_key_length(&ctx) == 16);
    OPENSSL_assert(EVP_CIPHER_CTX_iv_length(&ctx) == 16);

    /* Now we can set key and IV */
    EVP_CipherInit_ex(&ctx, NULL, NULL, key, iv, do_encrypt);

    inlen = strlen(in);
    int outFlag = 0 ; 

    /* update 可以被调用多次 */
    if(!EVP_CipherUpdate(&ctx, outbuf, &outlen, in, inlen))
    {
    
        EVP_CIPHER_CTX_cleanup(&ctx);
        return 0;
    }

    memcpy(*out, outbuf, outlen);
    /* 使用 flag 标志写到了字符数组的哪个位置 */
    outFlag += outlen ;

    /* final 生成加密结束必须的字节 */
    if(!EVP_CipherFinal_ex(&ctx, outbuf, &outlen))
    {
        EVP_CIPHER_CTX_cleanup(&ctx);
        return 0;
    }

    memcpy(*out+outFlag, outbuf, outlen);


    EVP_CIPHER_CTX_cleanup(&ctx);
    return 1;
}