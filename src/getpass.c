
#include <ft_ssl.h>
size_t ft_getpass(char** pass, int mode, char* name)
{
    size_t len;
    struct termios	term;
    int cmp = 0;
    char* verify = NULL;

    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~ICANON;
	term.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    if(mode == 0)
    {
        write(1, "enter ", 6);
        write(STDOUT_FILENO, name, ft_strlen(name));
        write(1, " encryption password: ", 22);
    }
    else
    {
        write(1, "enter ", 6);
        write(STDOUT_FILENO, name, ft_strlen(name));
        write(1, " decryption password: ", 22);
    }
    len = read_stdin(pass, true);
    if(mode == 0)
    {
        write(1, "\nreenter ", 9);
        write(STDOUT_FILENO, name, ft_strlen(name));
        write(1, " encryption password: ", 22);
        read_stdin(&verify, true);
        cmp = ft_strcmp(*pass, verify);
    }
    free(verify);

    term.c_lflag |= ICANON;
	term.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    if(cmp)
    {
        write(2, "\nVerify error, bad password\n", 28);
        exit(1);
    }
    write(1, "\n", 1);
    return(len);
}