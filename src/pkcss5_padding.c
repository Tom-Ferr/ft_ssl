#include <ft_ssl.h>

size_t pkcss5_padding(uint8_t** dest, uint8_t* src, int target)
{
    size_t o_len;
    size_t p_len;
    uint8_t pad;

    o_len = ft_veclen(src);
    p_len = o_len + 1;

    while (p_len % target)
        p_len++;
    *dest = (uint8_t*)malloc(p_len);

    pad = p_len - o_len;
    for (size_t i = 0; i < o_len; i++)
        (*dest)[i] = src[i];
    for (size_t i = o_len; i < p_len; i++)
        (*dest)[i] = pad;
    
    return(p_len);
}

size_t pkcss5_remove_pad(uint8_t** padded, size_t p_len)
{
    uint8_t pad;
    size_t i;
    uint8_t c;

    i = p_len - 1;
    c = 0;
    pad = (*padded)[i];
    while ((*padded)[i] == pad)
    {
        (*padded)[i] = '\0';
        i--;
        c++;
    }
    if(c == pad)
        return((size_t)c);
    return(0);
    
}