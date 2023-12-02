#include <ft_ssl.h>

off_t input_file(char* file, char** ret)
{
    int fd = open(file, O_RDONLY);
    if(fd == -1)
        return(-1);
    off_t size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    char* buff = (char*)malloc(size);; 
    read(fd, buff, size);
    close(fd);
    *ret = buff;
    return(size);
}