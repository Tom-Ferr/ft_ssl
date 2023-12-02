#include <ft_ssl.h>

static void help(char** argv)
{
    write(STDOUT_FILENO, "Usage: ", 7);
    write(STDOUT_FILENO, argv[1], ft_strlen(argv[1]));
    write(STDOUT_FILENO, " [options]\nValid options are:\n", 30);

    write(STDOUT_FILENO, "-help\t\tDisplay this summary\n", 28);
    write(STDOUT_FILENO, "-d\t\tDecrypt\n", 12);
    write(STDOUT_FILENO, "-e\t\tEncrypt\n", 12);
    write(STDOUT_FILENO, "-i\t\tInput file\n", 15);
    write(STDOUT_FILENO, "-o\t\tOutput file\n", 16);
    exit(0);
}

static t_base64_options read_options(char* argv[], int argc)
{
    int i = 2;
    t_base64_options ret;
    ret.in_file = NULL;
    ret.out_file = NULL;
    ret.options = 0;

    while(i < argc)
    {
        if(!ft_strcmp(*(argv + i), "-help"))
            help(argv);
        else if(!ft_strcmp(*(argv + i), "-i") && !ret.in_file)
        {
            if(*(argv + i + 1))
                ret.in_file = *(argv + i + 1);
            else
            {
                error_message(argv[1], *(argv + i), "' needs a value.\n");
                exit(1);
            }
            i += 1;
        }
        else if(!ft_strcmp(*(argv + i), "-o") && !ret.out_file)
        {
            if(*(argv + i + 1))
                ret.out_file = *(argv + i + 1);
            else
            {
                error_message(argv[1], *(argv + i), "' needs a value.\n");
                exit(1);
            }
            i += 1;
        }
        else if(!ft_strcmp(*(argv + i), "-d"))
            ret.options = 1;
        else if(!ft_strcmp(*(argv + i), "-e"))
            ret.options = 0;
        else
        {
            error_message(argv[1], *(argv + i), "' is an invalid option.\n\n");
            help(argv);
        }
        i++;
    }
    return(ret);
}

void base64_options(char** argv, int argc)
    {
        t_base64_options ask = read_options(argv, argc);
        char* base64_out;
        char* buff;
        size_t size;
        size_t size_out;
        int fd;

        if(ask.in_file)
        {
            off_t len = input_file(ask.in_file, &buff);
            if(len == -1)
            {
                write(STDOUT_FILENO, "ft_ssl: base64: ", 16);
                write(STDOUT_FILENO, ask.in_file, ft_strlen(ask.in_file));
                write(STDOUT_FILENO, ": No such file or directory\n", 28);
                return ;
            }
            size = (size_t)len;
        }
        else
            size = read_stdin(&buff, false);
        if(ask.options)
            base64_out = base64_decode((uint8_t*)buff, size, &size_out);
        else
        {
            base64_out = base64_encode((uint8_t*)buff, size);
            size_out = ft_strlen(base64_out);
        }
        if(ask.out_file)
            fd = open(ask.out_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
        else
            fd = STDOUT_FILENO;
        print_base64(fd, base64_out, size_out);
        if(fd != STDOUT_FILENO)
            close(fd);
        free(base64_out);
        free(buff);
    }