#include <ft_ssl.h>

void printfy_int_base(uint8_t* vec, uint8_t e, int base)
{
    uint64_t f;
    uint64_t g;
    size_t len;
    int i = sizeof(e) * 2;
    
    f = 0;
    len = 1;

    while (i)
    {
        f <<= 4;
        f |= e % base;
        e /= base;
        len++;
        i--;
    }
    while (--len)
    {
        g = (f & 0xf) + '0';
        if(g >= 58)
            g += 39;
        *vec++ = g;
        f >>= 4;   
    }
    
}

size_t hex_dump(uint8_t** rsa_out, size_t size)
{
    size_t line = (size_t)ft_ceil((double)size / 16);
    size_t total_size = (line * 64) + (line * (7 + 1)) + (line - 1);
    uint8_t* ret = (uint8_t*)malloc(total_size);
    for (size_t i = 0; i < total_size; i++)
        ret[i] = ' ';
    
    uint8_t* ret_0 = ret;
    uint8_t* ret_asc = NULL;

    for (size_t i = 0; i < size; i++)
    {
        if(!(i % 16))
        {
            if(i)
            {
                ret = ret_asc;
                *ret++ = '\n';
            }
            printfy_int_base(ret, i >> 8, 16);
            ret += 2;
            printfy_int_base(ret, (uint8_t)i, 16);
            ret += 5;
            *(ret-2) = '-';
            ret_asc = ret + (16 * 3);
            *ret_asc++ = '\t';
        }
        printfy_int_base(ret, (*rsa_out)[i], 16);
        ret += 2;
        if(i % 8 == 0 && i % 16 != 0)
            *ret = '-';
        ret++;
        uint8_t c = (*rsa_out)[i];
        if(c <= ' ' || c >= 0x7f)
            c = '.';
        *ret_asc++ =  c;
    }
    free(*rsa_out);
    *rsa_out = ret_0;
    return(total_size);
}