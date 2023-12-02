#include <ft_ssl.h>

char* format_cmd(char* in, size_t size)
{
    char* p = "ft_ssl ";
    char* ret = (char*)malloc(size + 7);

    for (size_t i = 0; i < 7; i++)
        ret[i] = p[i];
    
    for (size_t i = 0; i < size; i++)
    {
        ret[i+7] = in[i];
        if(in[i] == '\n')
            ret[i+7] = 0;
    }
    return(ret);
}