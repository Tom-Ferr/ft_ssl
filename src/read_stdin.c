#include <ft_ssl.h>

size_t read_stdin(char** ret, bool line)
{
    char buff[64];
    char* old = NULL;
    char* new = NULL;
    size_t size = 0;
    ssize_t status = 1;
    while(status > 0)
    {   
        status = read(STDIN_FILENO, buff, 64);
        size += status;
        old = new;
        new = (char*)malloc(size);
        if(old)
        {
            for (size_t i = 0; i < (size - status); i++)
                new[i] = old[i];
        }
        free(old);
        for (ssize_t i = 0; i < status; i++)
        {
            new[i + (size - status)] = buff[i];
            if(line && buff[i] == '\n')
            {
                new[i + (size - status)] = '\0';
                status = 0;
            }

        }

    }
    if(status == -1)
        return(status);
    *ret = new;
    return(size);
}