#include <ft_ssl.h>

static void help(char** argv)
{
    write(STDOUT_FILENO, "Usage: ", 7);
    write(STDOUT_FILENO, argv[1], ft_strlen(argv[1]));
    write(STDOUT_FILENO, " [options]\nValid options are:\n", 30);

    write(STDOUT_FILENO, "-help\t\tDisplay this summary\n", 28);
    write(STDOUT_FILENO, "-out\t\tOutput the key to specified file\n", 39);
    write(STDOUT_FILENO, "[64-4096]\tset modulus size in bits\n", 35);
    exit(0);
}

static t_genrsa_options read_options(char* argv[], int argc)
{
    int i = 2;
    t_genrsa_options ret;
    ret.in_file = NULL;
    ret.out_file = NULL;
    ret.bits = 64;

    while(i < argc)
    {
        if(!ft_strcmp(*(argv + i), "-help"))
            help(argv);
        else if(!ft_strcmp(*(argv + i), "-out") && !ret.out_file)
        {
            if(*(argv + i + 1))
                ret.out_file = *(argv + i + 1);
            else
            {
                error_message(argv[1], *(argv + i), "' needs a value.\n");
                exit(1);
            }
            i++;
        }
        else
        {
            uint32_t ctrl = mini_atoi(*(argv + i));
            if(ctrl >= 64 && ctrl <= 4096)
                ret.bits = ctrl;
            else
            {
                error_message(argv[1], *(argv + i), "' is an invalid option.\n\n");
                help(argv);
            }
        }
        i++;
    }
    return(ret);
}

void genrsa_options(char** argv, int argc)
{
        t_rsakey* key = (t_rsakey*)malloc(sizeof(t_rsakey));
        char* pem;
        int fd;

        t_genrsa_options ask = read_options(argv, argc);
        write(1, "Generating RSA private key, ", 28);
        put_int_base(1, ask.bits, 10);
        write(1, " bit long modulus\n", 18);
        key->p = prime_gen(ask.bits);
        if(ask.bits & 1)
            ask.bits += 1;
        key->q = prime_gen(ask.bits);
        while(bigint_compare(key->p, key->q) == 0)
        {
            free_bigint(key->q);
            key->q = prime_gen(ask.bits);
        }
        rsa_keygen(key);
        write(1, "e is ", 5);
        print_eval(1, key->e);
        t_der* der = key_to_der(key, RSA);
        uint8_t* str = format_der(der, PRIVATE);
        pem = base64_encode(str, der->size);
        if(ask.out_file)
            fd = open(ask.out_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
        else
            fd = STDOUT_FILENO;
        print_rsakey(fd, pem, PRIVATE, false, NULL);
        free_key(key, RSA);
        free_der(der, RSA);
        free(str);
        free(pem);
}