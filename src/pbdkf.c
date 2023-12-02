#include <ft_ssl.h>

uint8_t* pbdkf2(uint8_t* password,
                size_t p_len,
                uint8_t* salt,
                size_t c,
                size_t k_bytes,
                uint8_t* (*H)(uint8_t*, uint64_t),
                size_t h_bytes)
{
    if((k_bytes / 8) > (0x7ffffffffffff * (h_bytes / 8)))
        return(NULL);
    size_t len = ft_ceil((double)k_bytes / (double)h_bytes);
    // size_t r = k_bytes - (len - 1) * h_bytes;
    size_t salt_len = ft_veclen(salt);
    size_t u_len;
    size_t block_len;
    uint8_t* prev_u;
    uint8_t* u;
    uint8_t* t;
    size_t cursor;

    t = (uint8_t*)malloc(k_bytes);
    if(!t)
        return(NULL);

    prev_u = NULL;
    cursor = 0;

    for (size_t i = 1; i <= len; i++)
    {
        if(h_bytes < k_bytes - cursor)
            block_len = h_bytes;
        else
            block_len =  k_bytes - cursor;

        u_len = salt_len + sizeof(int);

        /*
            init T_i to 0
        */
        for (size_t k = 0; k < block_len; k++)    
            t[cursor + k] = 0;

        free(prev_u);
        prev_u = (uint8_t*)malloc(u_len);
        if(!prev_u)
            return(NULL);

        /*
            S || int(i)
        */
        for (size_t l = 0; l < salt_len; l++)
            prev_u[l] = salt[l];
        uint32_t i32 = (uint32_t)i;
        for (size_t l = 0; l < 4; l++)
            prev_u[salt_len + l] = (uint8_t)(i32 >> (32 - (8 * (l+1))));

        /*
            HMAC c times
        */
        for (size_t j = 0; j < c; j++)
        {
            u = hmac(prev_u, u_len, password, p_len, H, h_bytes, 64);
            u_len = h_bytes;

            for (size_t k = 0; k < block_len; k++)            
                t[cursor + k] ^= u[k];
            free(prev_u);
            prev_u = u;
        }
        cursor += h_bytes;
    }
    free(prev_u);
    return(t);
}