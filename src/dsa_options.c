#include <ft_ssl.h>

static void help(char** argv)
{
    write(STDOUT_FILENO, "Usage: ", 7);
    write(STDOUT_FILENO, argv[1], ft_strlen(argv[1]));
    write(STDOUT_FILENO, " [options]\nValid options are:\n", 30);

    write(STDOUT_FILENO, "-help\t\tDisplay this summary\n", 28);
    write(STDOUT_FILENO, "-in\t\tInput file\n", 16);
    write(STDOUT_FILENO, "-out\t\tOutput file\n", 18);
    write(STDOUT_FILENO, "-check\t\tVerify key consistency\n", 31);
    write(STDOUT_FILENO, "-pubout\t\tOutput a public key\n", 29);
    write(STDOUT_FILENO, "-pubin\t\tExpect a public key in input file\n", 42);
    write(STDOUT_FILENO, "-text\t\tPrint the key in text\n", 29);
    write(STDOUT_FILENO, "-modulus\t\tPrint the RSA key modulus\n", 36);
    write(STDOUT_FILENO, "-noout\t\tDon't print key out\n", 28);
    write(STDOUT_FILENO, "-passout\t\tOutput file pass phrase source\n", 41);
    write(STDOUT_FILENO, "-passin\t\tInput file pass phrase source\n", 39);
    write(STDOUT_FILENO, "-inform\t\tVerify key consistency\n", 32);
    write(STDOUT_FILENO, "-outform\t\tOutput format\n", 24);
    exit(0);
}

static t_rsa_options read_options(char* argv[], int argc)
{
    int i = 2;
    t_rsa_options ret;
    ret.in_file = NULL;
    ret.out_file = NULL;
    ret.password_in = NULL;
    ret.password_out = NULL;
    ret.options = 0;

    while(i < argc)
    {
        if(!ft_strcmp(*(argv + i), "-help"))
            help(argv);
        else if(!ft_strcmp(*(argv + i), "-in") && !ret.in_file)
        {
            if(*(argv + i + 1))
                ret.in_file = *(argv + i + 1);
            i++;
        }
        else if(!ft_strcmp(*(argv + i), "-out") && !ret.out_file)
        {
            if(*(argv + i + 1))
                ret.out_file = *(argv + i + 1);
            i++;
        }
        else if(!ft_strcmp(*(argv + i), "-passin") && !ret.password_in)
        {
            if(*(argv + i + 1))
                ret.password_in = *(argv + i + 1);
            i++;
        }
        else if(!ft_strcmp(*(argv + i), "-passout") && !ret.password_out)
        {
            if(*(argv + i + 1))
                ret.password_out = *(argv + i + 1);
            i++;
        }
        else if(!ft_strcmp(*(argv + i), "-pubin"))
            ret.options |= 0b11;
        else if(!ft_strcmp(*(argv + i), "-pubout"))
            ret.options |= 0b10;
        else if(!ft_strcmp(*(argv + i), "-text"))
            ret.options |= 0b100;
        else if(!ft_strcmp(*(argv + i), "-check"))
            ret.options |= 0b1000;
        else if(!ft_strcmp(*(argv + i), "-modulus"))
            ret.options |= 0b10000;
        else if(!ft_strcmp(*(argv + i), "-noout"))
            ret.options |= 0b100000;
        else if(!ft_strcmp(*(argv + i), "-des"))
            ret.options |= 0b1000000;
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

void dsa_options(char** argv, int argc)
{
        char* pem = NULL;
        char* buff = NULL;
        char* s_iv = NULL;
        char* password_in = NULL;
        char* password_out = NULL;
        off_t size;
        int fd;
        bool salted = false;

        t_dsakey* k = NULL;
        t_der* der = NULL;
        t_rsa_options ask = read_options(argv, argc);

        if(!ask.in_file)
            size = read_stdin(&buff, false);
        else
        {
            size = input_file(ask.in_file, &buff);
            if(size == -1)
            {
                write(STDOUT_FILENO, "ft_ssl: dsa: ", 16);
                write(STDOUT_FILENO, ask.in_file, ft_strlen(ask.in_file));
                write(STDOUT_FILENO, ": No such file or directory\n", 28);
                return ;
            }

        }
        if(ask.options & 1)
            size = parse_pem(&pem, buff, size, PUBLIC, &salted, &s_iv);
        else
            size = parse_pem(&pem, buff, size, DSA, &salted, &s_iv);
        if(pem)
        {
            if(salted)
            {
                size_t len;
                if(!ask.password_in)
                {
                    write(1, "enter des encryption password: ", 31);
                    len = read_stdin(&password_in, true);
                    ask.password_in = password_in;
                }
                else
                    len = ft_strlen(ask.password_in);
                size_t out_size;
                uint8_t* DK;
                uint8_t key[16];
                uint8_t iv[8];

                char* d_pem = base64_decode((uint8_t*)pem, size, &out_size);
                uint8_t* salt = parse_salted(&d_pem, out_size);
                out_size -= 16;

                DK = pbdkf2((uint8_t*)ask.password_in, len, salt, 1000, 24, sha256, 32);
                for (size_t i = 0; i < 16; i++)
                    key[i] = DK[i];
                for (size_t i = 0; i < 8; i++)
                    iv[i] = DK[i + 16];
                uint8_t* check_iv;
                size_t iv_size = atox(&check_iv, s_iv);
                if(ft_strlcmp((char*)check_iv, (char*)iv, iv_size))
                {
                    write(1, "bad decrypt\n", 12);
                    exit(1);
                }
                uint64_t* des_out = des((uint8_t*)d_pem, out_size, key, iv, DES_DECRYPT);
                uint8_t* vec = hash_to_vec(des_out, out_size);
                size = sizeof(uint64_t) * (out_size / 8);
                free(pem);
                free(d_pem);
                free(check_iv);
                free(salt);
                free(des_out);
                free(DK);
                pem = (char*)vec;
                salted = false;
            }
            der = decode_pem(pem, size, DSA);
            k = (t_dsakey*)der_to_key(der, DSA);
            if(ask.options & 1)
            {
                if((ask.options & 0b100) == 0b100)
                    print_key_values(1, der, PUBLIC);
            }
            else
            {
                if((ask.options & 0b100) == 0b100)
                    print_key_values(1, der, PRIVATE);
            }
            // if((ask.options & 0b1000) == 0b1000)
            // {
            //     if(ask.options & 1)
            //     {
            //         write(STDOUT_FILENO, "Only private keys can be checked", 32);
            //         write(STDOUT_FILENO, "\n", 1);
            //         free(pem);
            //         free(k);
            //         return ;
            //     }
            //     else
            //         check_rsakey(k);
            // }
            // if((ask.options & 0b10000) == 0b10000)
            //     print_modulus(1, k);
            if(ask.options & 0b1000000)
            {
                size_t len;
                if(!ask.password_out)
                {
                    char* verify;
                    write(1, "enter des encryption password: ", 31);
                    len = read_stdin(&password_out, true);
                    write(1, "reenter des encryption password: ", 33);
                    read_stdin(&verify, true);
                    if(ft_strcmp(password_out, verify))
                    {
                        write(2, "Verify error, bad password", 26);
                        exit(1);
                    }
                    free(verify);
                    ask.password_out = password_out;
                }
                else
                    len = ft_strlen(ask.password_out);
                uint8_t* DK;
                uint8_t* padded;
                uint8_t key[16];
                uint8_t iv[8];

                uint64_t s = ft_urand();
                uint8_t* salt = i64tov8(s);

                DK = pbdkf2((uint8_t*)ask.password_out, len, salt, 1000, 24, sha256, 32);
                for (size_t i = 0; i < 16; i++)
                    key[i] = DK[i];
                s_iv = (char*)malloc(16+1);
                for (size_t i = 0; i < 16; i+= 2)
                {
                    iv[i/2] = DK[(i/2) + 16];
                    s_iv[i] = (iv[i/2] >> 4) + '0';
                    if(s_iv[i] >= 58)
                        s_iv[i] += 7;
                    s_iv[i+1] = (iv[i/2] & 0xf) + '0';
                    if(s_iv[i+1] >= 58)
                        s_iv[i+1] += 7;
                }
                s_iv[16] = 0;
                
                size = pkcss5_padding(&padded, (uint8_t*)pem, 8);
                uint64_t* des_out = des(padded, size, key, iv, DES_ENCRYPT);
                uint8_t* vec = hash_to_vec(des_out, size);
                size = sizeof(uint64_t) * (size / 8);
                uint8_t salt_head[8] = "Salted__";
                uint8_t* tmp = (uint8_t*)malloc(size + 16);
                for (off_t i = 0; i < 8; i++)
                    tmp[i] = salt_head[i];
                for (off_t i = 0; i < 8; i++)
                    tmp[i+8] = salt[i];
                for (off_t i = 0; i < size; i++)
                    tmp[i+16] = vec[i];
                size += 16;
                char* e_pem = base64_encode((uint8_t*)tmp, size);
                free(pem);
                free(padded);
                free(des_out);
                free(vec);
                free(tmp);
                free(salt);
                free(DK);
                pem = e_pem;
                salted = true;
            }
            if(ask.out_file)
                fd = open(ask.out_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
            else
                fd = STDOUT_FILENO;
            if((ask.options & 0b100000) == 0)
            {
                write(STDOUT_FILENO, "writing RSA key\n", 16);
                if((ask.options & 0b10) == 0b10)
                {
                    uint8_t* pb = format_der(der, DSA);
                    char* text = base64_encode(pb, der->size);
                    print_rsakey(fd, text, DSA, salted, s_iv);
                    free(text);
                    free(pb);
                }
                else
                    print_rsakey(fd, pem, PRIVATE, salted, s_iv);
            }
            free(pem);
            free(buff);
            free(password_in);
            free(password_out);
            free_der(der, DSA);
            free_key(k, DSA);
        }
}