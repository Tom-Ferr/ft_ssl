#include <ft_ssl.h>

static void help(char** argv)
{
    write(STDOUT_FILENO, "Usage: ", 7);
    write(STDOUT_FILENO, argv[1], ft_strlen(argv[1]));
    write(STDOUT_FILENO, " [options]\nValid options are:\n", 30);

    write(STDOUT_FILENO, "-help\t\tDisplay this summary\n", 28);
    write(STDOUT_FILENO, "-out\t\tOutput the key to specified file\n", 39);
    exit(0);
}

static t_genrsa_options read_options(char* argv[], int argc)
{
    int i = 2;
    t_genrsa_options ret;
    ret.in_file = NULL;
    ret.out_file = NULL;
    ret.bits = 1024;

    while(i < argc)
    {
        if(!ft_strcmp(*(argv + i), "-help"))
            help(argv);
        else if(!ft_strcmp(*(argv + i), "-out") && !ret.out_file)
        {
            ret.out_file = *(argv + i + 1);
            i++;
        }
        else
        {
        
           write(STDOUT_FILENO, "ft_ssl: ", 8);
           write(STDOUT_FILENO, argv[1], ft_strlen(argv[1]));
           write(STDOUT_FILENO, ": Error: '", 10);
           write(STDOUT_FILENO, *(argv + i), ft_strlen(*(argv + i)));
           write(STDOUT_FILENO, "' is an invalid option.\n\n", 25);
           help(argv);
        
        }
        i++;
    }
    return(ret);
}

void gendes_options(char** argv, int argc)
{
        int fd;

        t_genrsa_options ask = read_options(argv, argc);
        write(1, "Generating DES key\n", 19);

        uint64_t key = ft_urand();

        if(ask.out_file)
            fd = open(ask.out_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
        else
            fd = STDOUT_FILENO;
        uint8_t* out = i64tov8(key);
        print_hash(fd, out, 64);
        write(1, "\n", 1);
        free(out);
}
