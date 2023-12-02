#include <ft_ssl.h>

static void help(char** argv)
{
    write(STDOUT_FILENO, "Usage: ", 7);
    write(STDOUT_FILENO, argv[1], ft_strlen(argv[1]));
    write(STDOUT_FILENO, " [options]\nValid options are:\n", 30);

    write(STDOUT_FILENO, "-help\t\tDisplay this summary\n", 28);
    write(STDOUT_FILENO, "-in\t\tInput file\n", 16);
    write(STDOUT_FILENO, "-out\t\tOutput file\n", 18);
    write(STDOUT_FILENO, "-pubin\t\tInput is an RSA public key\n", 35);
    write(STDOUT_FILENO, "-inkey\t\tInput key\n", 18);
    write(STDOUT_FILENO, "-hexdump\tHex dump output\n", 25);
    write(STDOUT_FILENO, "-encrypt\tEncrypt with public key\n", 33);
    write(STDOUT_FILENO, "-decrypt\tDecrypt with private key\n", 34);
    exit(0);
}

static t_rsautl_options read_options(char* argv[], int argc)
{
    int i = 2;
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
        else if(!ft_strcmp(*(argv + i), "-pubin"))
            ret.options |= 0b01;
        else if(!ft_strcmp(*(argv + i), "-encrypt"))
            ret.options |= 0b10;
        else if(!ft_strcmp(*(argv + i), "-decrypt"))
            ret.options |= 0b100;
        else if(!ft_strcmp(*(argv + i), "-hexdump"))
            ret.options |= 0b1000;
        else
        {
            error_message(argv[1], *(argv + i), "' is an invalid option.\n\n");
            help(argv);
        }
        i++;
    }
    return(ret);
}

void rsautl_options(char** argv, int argc)
    {
        char* pem = NULL;
        int fd;
        char* buff;
        t_bigint* rsa_out;
        uint8_t* rsa_str = NULL;
        size_t out_size;
        size_t size;
        off_t length;
        size_t pem_size;
        t_der* der = NULL;
        int vis;
        char* s_iv = NULL;
        char* password_in = NULL;
        bool salted = false;

        t_rsakey* k = NULL;
        t_rsautl_options ask = read_options(argv, argc);

        if(!ask.key)
        {
            error_message(argv[1], NULL, ": RSA key is needed\n");
            return ;
        }
        if((ask.options & 0b110) == 0)
        {
            error_message(argv[1], NULL, ": encryption mode is needed\n");
            return ;
        }
        if((ask.options & 0b101) == 0b101)
        {
            error_message(argv[1], NULL, ": A private key is needed for this operation\n");
            return ;
        }

        length = input_file(ask.key, &buff);
        if(length == -1)
        {
            error_message(argv[1], ask.key, "': No such file or directory\n");
            return ;
        }
        size = (size_t)length;

        if(ask.options & 1)
            vis = PUBLIC;
        else
            vis = PRIVATE;
        pem_size = parse_pem(&pem, buff, size, vis, &salted, &s_iv);
        if(pem)
        {
            if(salted)
            {
                size_t len;
                if(!ask.password_in)
                {
                    len = ft_getpass(&password_in, 1, "des");
                    ask.password_in = password_in;
                }
                else
                    len = ft_strlen(ask.password_in);
                size_t out_size;
                uint8_t* DK;
                uint8_t key[16];
                uint8_t iv[8];

                char* d_pem = base64_decode((uint8_t*)pem, pem_size, &out_size);
                uint8_t* salt = parse_salted(&d_pem, out_size);
                out_size -= 16;

                DK = pbdkf2((uint8_t*)ask.password_in, len, salt, 10000, 24, sha256, 32);
                for (size_t i = 0; i < 16; i++)
                    key[i] = DK[i];
                for (size_t i = 0; i < 8; i++)
                    iv[i] = DK[i + 16];
                uint8_t* check_iv;
                size_t iv_size = atox(&check_iv, s_iv);
                if(ft_strlcmp((char*)check_iv, (char*)iv, iv_size))
                {
                    error_message(argv[1], ask.in_file, "': bad decrypt\n");
                    exit(1);
                }
                uint64_t* des_out = des((uint8_t*)d_pem, out_size, key, iv, DES_DECRYPT);
                out_size -= pkcss5_remove_pad((uint8_t**)(&des_out), out_size);
                uint8_t* vec = hash_to_vec(des_out, out_size);
                pem_size = sizeof(uint64_t) * (out_size / 8);
                free(pem);
                free(d_pem);
                free(check_iv);
                free(salt);
                free(des_out);
                free(DK);
                pem = (char*)vec;
                salted = false;
            }
            free(buff);
            if(ask.in_file)
            {
                off_t len = input_file(ask.in_file, &buff);
                if(len == -1)
                {
                    error_message(argv[1], ask.in_file, "': No such file or directory\n");
                    return ;
                }
                size = (size_t)len;
            }
            else
                size = read_stdin(&buff, false);

            der = decode_pem(pem, pem_size, vis);
            k = (t_rsakey*)der_to_key(der, vis);
            if((ask.options & 0b10) == 0b10)
            {
                size_t bit_size = getkey_bitsize(der);
                if(bit_size >= 1024)
                {
                    uint8_t* em = oaep_encode((uint8_t*)buff, size, NULL, (bit_size / 8) - 1, sha256, 32);
                    size = (bit_size / 8) - 1;
                    free(buff);
                    buff = (char*)em;
                    if(!em)
                    {
                        error_message(argv[1], NULL, ": message too long\n");
                        exit(1);
                    }
                }
                rsa_out = rsa_encode((uint8_t*)buff, size, *k);
                out_size = bigint_to_string(&rsa_str, rsa_out);
                free_bigint(rsa_out);
            }
            if((ask.options & 0b100) == 0b100)
            {
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
            }
            if((ask.options & 0b1000) == 0b1000)
                out_size = hex_dump(&rsa_str, out_size);
            if(ask.out_file)
                fd = open(ask.out_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
            else
                fd = STDOUT_FILENO;
            write(fd, rsa_str, out_size);
            
            free(rsa_str);
            free(pem);
            free_der(der, vis);
            free_key(k, vis);
            free(password_in);
        }
        free(buff);
    }