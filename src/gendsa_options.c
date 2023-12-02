#include <ft_ssl.h>

static void help(char** argv)
{
    write(STDOUT_FILENO, "Usage: ", 7);
    write(STDOUT_FILENO, argv[1], ft_strlen(argv[1]));
    write(STDOUT_FILENO, " [options]\nValid options are:\n", 30);

    write(STDOUT_FILENO, "-help\t\tDisplay this summary\n", 28);
    write(STDOUT_FILENO, "-out\t\tOutput the key to specified file\n", 39);
    write(STDOUT_FILENO, "[1024-4096]\tset modulus size in bits\n", 37);
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
            uint32_t ctrl = mini_atoi(*(argv + i));
            if(ctrl >= 1024 && ctrl <= 4096)
                ret.bits = ctrl;
            else
            {
                write(STDOUT_FILENO, "ft_ssl: ", 8);
                write(STDOUT_FILENO, argv[1], ft_strlen(argv[1]));
                write(STDOUT_FILENO, ": Error: '", 10);
                write(STDOUT_FILENO, *(argv + i), ft_strlen(*(argv + i)));
                write(STDOUT_FILENO, "' is an invalid option.\n\n", 25);
                help(argv);
            }
        }
        i++;
    }
    return(ret);
}

void gendsa_options(char** argv, int argc)
{
        t_dsakey* key;
        char* pem;
        int fd;

        t_genrsa_options ask = read_options(argv, argc);
        write(1, "Generating DSA key, ", 20);
        put_int_base(1, ask.bits, 10);
        write(1, " bits\n", 6);

        key = gen_dsa_primes(1024,160, 160);
        gen_dsa_generator(key);
        gen_dsa_key(key);
        t_der* der = key_to_der(key, DSA);
        uint8_t* str = format_der(der, DSA);
        pem = base64_encode(str, der->size);
        if(ask.out_file)
            fd = open(ask.out_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
        else
            fd = STDOUT_FILENO;
        print_rsakey(fd, pem, DSA, false, NULL);
        free_key(key, DSA);
        free_der(der, DSA);
        free(str);
        free(pem);
}