#include <ft_ssl.h>
static uint8_t ctox(char* str)
{
    if (*str >= '0' && *str <= '9')
        return(*str - 48);
    else if(*str >= 'A' && *str <= 'Z')
        return(*str - 55);
    else if(*str >= 'a' && *str <= 'z')
        return(*str - 87);
    return(0xff);
}

static size_t check(char* str)
{
    size_t i;

    i = 0;
    while(str[i])
    {
        if (str[i] >= '0' && str[i] <= '9')
            i++;
        else if(str[i] >= 'A' && str[i] <= 'F')
            i++;
        else if(str[i] >= 'a' && str[i] <= 'f')
            i++;
        else
            return(0);
    }
    return(i);
}
size_t atox(uint8_t** vec, char* str)
{
    size_t len;
    uint8_t x;
    size_t i;

    if(str == NULL)
        return(0);
    len = check(str);
    if(!len)
        return(0);
    if(len % 2)
    {
        i = 1;
        len = (len + 1) / 2 ;
        *vec = (uint8_t*)malloc(len);
        x = ctox(str);
        (*vec)[0] = 0;
        (*vec)[0] |= x;
        str++;
    }
    else
    {
        i = 0;
        len /= 2;
        *vec = (uint8_t*)malloc(len);
    }
    for (; i < len; i++)
    {
        x = ctox(str);

        (*vec)[i] = x;
        str++;    

        x = ctox(str);

        (*vec)[i] <<= 4;
        (*vec)[i] |= x;
        str++;
    }

    return(i);
}