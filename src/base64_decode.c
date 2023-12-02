#include<ft_ssl.h>

static uint8_t get_index(uint8_t c)
{
    uint8_t index;

    if(c >= 'A' && c <= 'Z')
        index = c - 'A';
    else if(c >= 'a' && c <= 'z')
        index = (c - 'a') + 26;
    else if(c >= '0' && c <= '9')
        index = (c - '0') + 52;
    else if(c == '+')
        index = 62;
    else if(c == '/')
        index = 63;
    else if(c == '\n')
        index = 0x7f;
    else
        index = 0xff;
    return(index);
}

static void build_index_buffer(uint8_t* cypher, uint8_t* index)
{
    while (*cypher)
    {
        *index = get_index(*cypher);
        cypher++;
        if(*index != 0x7f)
            index++;
    }
    *index = 0xff;
}

static char* join_bits8(uint8_t* index, char* plain)
{
    uint32_t a;
    uint32_t b;
    uint32_t z;
    size_t i;

    i = 0;
    while(*index != 0xff)
    {
        if(!((i +1) % 4))
        {
            i = 0;
            index++;
            continue;
        }
        a = (uint32_t)(*index);
        a = a << 6;
        b = (uint32_t)(*(index+1));
        z = (a | b);
        *plain = (char)(z >> (((4 * (i+1)) % 6)));
        i++;
        index++;
        plain++;
    }
    if(i)
        plain -= 1;
    *(plain) = '\0';
    return(plain);
}

char* base64_decode(uint8_t* cypher, size_t len, size_t* out_size)
{
    uint8_t* index;
    char* plain;
    
    index = (uint8_t*)malloc((len + 1) * sizeof(uint8_t));
    if(!index)
        return(NULL);

    plain = (char*)malloc((len) * sizeof(char));
    if(!index)
        return(NULL);

    build_index_buffer(cypher, index);

    *out_size = (join_bits8(index, plain) - plain);

    free(index);
    return(plain);
}