#include <ft_ssl.h>

static size_t measure_padding(const uint64_t o_len, const uint64_t n_bits, const uint64_t length_field)
{
    
    uint64_t n;
    uint64_t d;

    d = (o_len + length_field) / n_bits;
    n = d + 1;

    return(n * n_bits);
}

static void copy_message(uint8_t* ret, const uint8_t* plain_text, uint64_t o_len)
{
    size_t i;

    i = 0;
    while(i < o_len)
    {
        ret[i] = plain_text[i];
        i++;
    }
}

static void set_padding(uint8_t* padding, const size_t len, uint64_t i)
{
    padding[i] = 0x80;

    i++;
    while(i < len)
    {
        padding[i] = 0x0;
        i++;
    }
}

uint64_t* add_padding(const uint8_t* plain_text, uint64_t* total_len, const uint64_t n_bits, const uint64_t length_field)
{
    uint64_t o_len;
    uint64_t padded_len;
    uint8_t* tmp;
    uint64_t* ret;

    o_len = *total_len;
    *total_len = measure_padding(o_len, n_bits, length_field);

    padded_len = *total_len - length_field;
    tmp = (uint8_t*)malloc(*total_len);
    if(!tmp)
        return(NULL);
    
    copy_message(tmp, plain_text, o_len);
    set_padding(tmp, *total_len, o_len);
    
    ret = (uint64_t*)tmp;
    ret[padded_len / sizeof(uint64_t)] = (o_len * 8);

    return(ret);
} 