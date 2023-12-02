#include <ft_ssl.h>

size_t length_field_bytes(size_t i)
{
    size_t n;

    n = 1;
    while (i)
    {
        if(i >= 0x80)
            n++;
        i >>= 8;
    }
    return(n);
}

size_t required_bytes(size_t length)
{
    size_t t = 1;
    size_t n = length_field_bytes(length);

    if(n > 1)
        n++;
    t += n;
    return(t + length);
}

size_t calculate_der_size(t_der* der, int vis)
{
    size_t sequence_size = 0;
    uint8_t** iter_der = (uint8_t**)der;
    if(vis != PUBLIC)
        sequence_size += 3;
    for (int i = 1; i < vis; i += 2)
        sequence_size += required_bytes((size_t)iter_der[i]);
    der->sequence_length = sequence_size;

    sequence_size = required_bytes(sequence_size);

    return(sequence_size);
}

void format_length(uint8_t** dest, size_t length)
{
    if(length >= 0x80)
    {
        *(*dest) = 0x80;
        uint8_t n = length_field_bytes(length);
        *(*dest)++ |= n;
        for(; n > 0; n--)
            *(*dest)++ = length >> ((n-1) * 8);
    }
    else
        *(*dest)++ = length;
}

void format_integer(uint8_t** dest, uint8_t* src, size_t length)
{
    *(*dest)++ = 0x2;
    format_length(dest, length);
    for(size_t j = 0; j < length; j++)
       *(*dest)++ = src[j];
}


uint8_t* format_der(t_der* der, int vis)
{
    der->size = calculate_der_size(der, vis);
    uint8_t** iter_der = (uint8_t**)der;

    uint8_t* f = (uint8_t*)malloc(der->size);
    uint8_t* f_0 = f;

    *f++ = 0x30;
    format_length(&f, der->sequence_length);
    /*
        Version
    */
    if(vis != PUBLIC)
    {
        *f++ = 0x2;
        *f++ = 0x1;
        *f++ = 0x0;
    }
    /*
        RSA_VALUES
    */
    for (int i = 0; i < vis; i += 2)
        format_integer(&f, iter_der[i], (size_t)iter_der[i+1]);
    
    return(f_0);
}