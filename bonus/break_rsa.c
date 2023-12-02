#include <ft_ssl.h>

static void help(char** argv)
{
    write(STDOUT_FILENO, "Usage: ", 7);
    write(STDOUT_FILENO, argv[1], ft_strlen(argv[1]));
    write(STDOUT_FILENO, " [options]\nValid options are:\n", 30);

    write(STDOUT_FILENO, "-help\t\tDisplay this summary\n", 28);
    write(STDOUT_FILENO, "-in\t\tInput file\n", 16);
    write(STDOUT_FILENO, "-out\t\tOutput file\n", 18);
    write(STDOUT_FILENO, "-inkey\t\tInput key\n", 18);
    exit(0);
}

static t_rsautl_options read_options(char* argv[], int argc)
{
    int i = 1;
    t_rsautl_options ret;
    ret.in_file = NULL;
    ret.out_file = NULL;
    ret.key = NULL;
    ret.password_in = NULL;
    ret.options = 0;

    while(i < argc)
    {
        if(!ft_strcmp(*(argv + i), "-help"))
            help(argv);
        else if(!ft_strcmp(*(argv + i), "-in") && !ret.in_file)
        {
            if(*(argv + i + 1))
                ret.in_file = *(argv + i + 1);
            else
            {
                error_message(argv[1], *(argv + i), "' needs a value.\n");
                exit(1);
            }
            i++;
        }
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
        else if(!ft_strcmp(*(argv + i), "-inkey") && !ret.key)
        {
            if(*(argv + i + 1))
                ret.key = *(argv + i + 1);
            else
            {
                error_message(argv[1], *(argv + i), "' needs a value.\n");
                exit(1);
            }
            i++;
        }
        else
        {
            error_message(argv[0], *(argv + i), "' is an invalid option.\n\n");
            help(argv);
        }
        i++;
    }
    return(ret);
}

int main(int argc, char* argv[])
{
    t_bigint* a;
    t_bigint* a2;
    t_bigint* b;
    t_bigint* b2;
    t_bigint* sqr;
    t_bigint* n;
    t_bigint* e;
    char* pem = NULL;
    char* buff = NULL;
    char* s_iv = NULL;
    off_t size;
    int fd;
    bool salted = false;
    t_rsakey* k = NULL;
    t_der* der = NULL;
    t_bigint* rsa_out;
    uint8_t* rsa_str = NULL;
    size_t out_size;

    t_rsautl_options ask = read_options(argv, argc);

    if(!ask.key)
    {
        error_message(argv[1], NULL, ": RSA key is needed\n");
        exit(1);
    }

    size = input_file(ask.key, &buff);
    if(size == -1)
    {
        error_message(argv[1], ask.key, "': No such file or directory\n");
        exit(1);
    }
    size = parse_pem(&pem, buff, size, PUBLIC, &salted, &s_iv);
    if(pem)
    {
        der = decode_pem(pem, size, PUBLIC);

        n = string_to_bigint((char*)der->n, der->l_n);
        e = string_to_bigint((char*)der->e, der->l_e);
        a = bigint_sqrt(n);
        bigint_add_int(a,1);

        write(1, "factorizing public key...\n", 26);
        write(1, "This may take a while\n", 22);
        while (true)
        {
            a2 = bigint_mul(a,a);
            b2 = bigint_sub(a2,n);
            b = bigint_sqrt(b2);
            sqr = bigint_mul(b,b);
            if(bigint_compare(sqr, b2) == 0)
            {
                free_bigint(a2);
                free_bigint(b2);
                free_bigint(sqr);
                break;
            }
            free_bigint(a2);
            free_bigint(b2);
            free_bigint(b);
            free_bigint(sqr);
            bigint_add_int(a,1);
        }
        k = ((t_rsakey*)malloc(sizeof(t_rsakey)));
        k->p = bigint_add(a,b);
        k->q = bigint_sub(a,b);

        free_bigint(n);
        free_bigint(e);
        free_bigint(a);
        free_bigint(b);

        rsa_keygen(k);
        check_rsakey(k);

        free(pem);
        free(buff);
        free(s_iv);
        free_der(der, PUBLIC);

        der = key_to_der(k, RSA);
        uint8_t* str = format_der(der, PRIVATE);
        pem = base64_encode(str, der->size);

        if(ask.out_file)
            fd = open(ask.out_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
        else
            fd = STDOUT_FILENO;

        if(ask.in_file)
        {
            off_t len = input_file(ask.in_file, &buff);
            if(len == -1)
            {
                error_message(argv[1], ask.in_file, "': No such file or directory\n");
                exit(1);
            }
            size = (size_t)len;

            rsa_out = rsa_decode((uint8_t*)buff, size, *k);
            out_size = bigint_to_string(&rsa_str, rsa_out);

            if(out_size >= 127)
            {
                uint8_t* m = oaep_decode(rsa_str, out_size, NULL, sha256, 32);
                free(rsa_str);
                rsa_str = m;
                if(!m)
                {
                    error_message(argv[1], NULL, ": decryption error\n");
                    exit(1);
                }
                out_size = ft_veclen(m);
            }
            free_bigint(rsa_out);
            write(fd, rsa_str, out_size);
            free(rsa_str);
            free(buff);
        }
        else
            print_rsakey(fd, pem, PRIVATE, salted, s_iv);

        free_key(k, RSA);
        free_der(der, PRIVATE);
        free(str);
        free(pem);
    }

    return(0);
}