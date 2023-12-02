#include <ft_ssl.h>

uint8_t* parse_salted(char** src, size_t size)
{
    char salted_head[8] = "Salted__";
    for (size_t i = 0; i < 8; i++)
    {
        if(salted_head[i] != (*src)[i])
            return(NULL);
    }
    
    char* salt = malloc(8);
    char* cypher = malloc(size-8);
    for (size_t i = 0; i < 8; i++)
        salt[i] = (*src)[i+8];
    for (size_t i = 0; i < size-8; i++)
        cypher[i] = (*src)[i+16];
    free(*src);
    *src = cypher;
    return((uint8_t*)salt);
}