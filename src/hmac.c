#include <ft_ssl.h>

uint8_t* key_pad(uint8_t* k, size_t len, size_t block_size)
{
    uint8_t* ret = (uint8_t*)malloc(block_size);
    size_t i = 0;
    for (; i < len; i++)
        ret[i] = k[i];
    for (; i < block_size; i++)
        ret[i] = 0;
    return(ret);
    
}

static uint8_t* hash_pad(uint8_t pad, uint8_t* k, uint8_t* suffix, size_t len, uint8_t* (*H)(uint8_t*,uint64_t), size_t block_size)
{

    uint8_t* prefix = (uint8_t*)malloc(block_size + len);

    for (size_t i = 0; i < block_size; i++)
        prefix[i] = k[i] ^ pad;

    for (size_t i = 0; i < len; i++)
        prefix[block_size + i] = suffix[i];

    uint8_t* hash = H(prefix, block_size + len);
    free(prefix);
    return(hash);
}

uint8_t* hmac(uint8_t* plain, size_t p_bytes, uint8_t* k, size_t k_bytes, uint8_t* (*H)(uint8_t*,uint64_t), size_t h_bytes, size_t block_size)
{
    uint8_t* hash0;
    uint8_t* hash1;
    uint8_t* k_0;

    if(k_bytes == block_size)
        k_0 = k;
    else if(k_bytes < block_size)
        k_0 = key_pad(k, k_bytes, block_size);
    else
    {
        uint8_t* k_hash = H(k, k_bytes);
        k_0 = key_pad(k_hash, h_bytes, block_size);
        free(k_hash);
    }
    hash0 = hash_pad(0x36, k_0, plain, p_bytes, H, block_size);
    hash1 = hash_pad(0x5c, k_0, hash0, h_bytes, H, block_size);
    if(k != k_0)
        free(k_0);
    free(hash0);
    return(hash1);
}