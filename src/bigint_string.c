#include<ft_ssl.h>

t_bigint* string_to_bigint(char* string, size_t len)
{
    t_bigint* bg;

    bg = init_bigint(0);
    while(len--)
    {
        bigint_left_shift(bg, 8);
        bigint_add_int(bg, (uint8_t)*string);
        string++;
    }
    return(bg);
}

size_t bigint_to_string(uint8_t** str, t_bigint* bg)
{
    uint64_t e;
    uint64_t z;
    uint64_t* f;
    uint64_t ctrl = false;
    size_t size = 0;
    size_t st_len = 0;
    uint8_t* str_0;

    f = (uint64_t*)malloc(bg->n * sizeof(uint64_t));
    for (size_t i = 0; i < bg->n; i++)
    {
        e = bg->len[i];
        f[i] = 0;
        z = e;
        if(ctrl)
            z = ~0UL;
        while (z)
        {
            f[i] <<= 4;
            f[i] |= e & 0xf;
            e >>= 4;
            z >>= 4;
            f[i] <<= 4;
            f[i] |= e & 0xf;
            e >>= 4;
            z >>= 4;
            size++;
            if(ctrl == false)
                st_len++;
        }
         if(bg->len[i])
            ctrl = true;
    }
    *str = (uint8_t*)malloc(size);
    str_0 = *str;
    ctrl = false;
    for (size_t i = 0; i < bg->n; i++)
    {
        e = f[i];
        z = f[i];
        if(ctrl)
            z = ~0UL;
        while (z)
        {
            **str = (e & 0xf) << 4;
            e >>= 4;
            **str |= e & 0xf;
            e >>= 4;
            (*str)++;
            z >>= 8;
            if(ctrl	== false)
            {
                st_len--;
                z = st_len;
            }
        }
        if(f[i])
            ctrl = true;
    }
    *str = str_0;
    free(f);
    return(size);
}