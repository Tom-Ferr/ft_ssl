#include <ft_ssl.h>

void print_base64(int fd, char* text, size_t len)
{
    size_t i;

    i = 1;
    while( i <= len)
    {
        if(*text != '\n')
        {
            write(fd, text, 1);
            if((i % 64) == 0)
                write(fd, "\n", 1);
        }
        i++;
        text++;
    }
    if(((i-1) % 64) != 0)
        write(fd, "\n", 1);
}