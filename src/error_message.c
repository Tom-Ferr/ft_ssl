#include <ft_ssl.h>

void error_message(char* cmd, char* opt, char* message)
{
    write(STDOUT_FILENO, "ft_ssl: ", 8);
    write(STDOUT_FILENO, cmd, ft_strlen(cmd));
    write(STDOUT_FILENO, ": Error", 7);
    if(opt)
    {
        write(STDOUT_FILENO, ": '", 3);
        write(STDOUT_FILENO, opt, ft_strlen(opt));
    }
    write(STDOUT_FILENO, message, ft_strlen(message));
}