#include <ft_ssl.h>

static void help(char** argv)
{
    write(STDOUT_FILENO, "Usage: ", 7);
    write(STDOUT_FILENO, argv[1], ft_strlen(argv[1]));
    write(STDOUT_FILENO, " [options]\nValid options are:\n", 30);

    write(STDOUT_FILENO, "-help\t\tDisplay this summary\n", 28);
    write(STDOUT_FILENO, "-a\t\tBase64 encode/decode, depending on encryption flag\n", 55);
    write(STDOUT_FILENO, "-d\t\tDecrypt\n", 12);
    write(STDOUT_FILENO, "-e\t\tEncrypt\n", 12);
    write(STDOUT_FILENO, "-i\t\tInput file\n", 15);
    write(STDOUT_FILENO, "-k\t\tRaw key, in hex\n", 20);
    write(STDOUT_FILENO, "-o\t\tOutput file\n", 16);
    write(STDOUT_FILENO, "-p\t\tPassword\n",13);
    write(STDOUT_FILENO, "-s\t\tSalt, in hex\n", 17);
    write(STDOUT_FILENO, "-v\t\tIV in hex\n", 14);
    write(STDOUT_FILENO, "-P\t\tPrint the iv/key\n", 21);
    exit(0);
}

static t_des_options read_options(char* argv[], int argc)
{
    int i = 2;
    t_des_options ret;
    ret.in_file = NULL;
    ret.out_file = NULL;
    ret.password = NULL;
    ret.key = NULL;
    ret.salt = NULL;
    ret.iv = NULL;
    ret.options = 0;

    if(!ft_strcmp(*(argv + 1), "des") || !ft_strcmp(*(argv + 1), "des-cbc"))
        ret.options = 0b100;
    else if(!ft_strcmp(*(argv + 1), "des-ecb"))
        ret.options = 0b1000;
    else if(!ft_strcmp(*(argv + 1), "des-cfb"))
        ret.options = 0b10000;
    else if(!ft_strcmp(*(argv + 1), "des-ofb"))
        ret.options = 0b100000;
    else if(!ft_strcmp(*(argv + 1), "des-ctr"))
        ret.options = 0b1000000;
    else if(!ft_strcmp(*(argv + 1), "des3") || !ft_strcmp(*(argv + 1), "des3-cbc"))
        ret.options = 0b10000100;
    else if(!ft_strcmp(*(argv + 1), "des3-ecb"))
        ret.options = 0b10001000;
    else if(!ft_strcmp(*(argv + 1), "des3-cfb"))
        ret.options = 0b10010000;
    else if(!ft_strcmp(*(argv + 1), "des3-ofb"))
        ret.options = 0b10100000;
    else if(!ft_strcmp(*(argv + 1), "des3-ctr"))
        ret.options = 0b11000000;
    else if(!ft_strcmp(*(argv + 1), "des-pcbc") || !ft_strcmp(*(argv + 1), "des3-pcbc"))
        ret.options = 0b1000000100;

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
            i++;
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
            i++;
        }
        else if(!ft_strcmp(*(argv + i), "-p") && !ret.password)
        {
            if(*(argv + i + 1))
                ret.password = *(argv + i + 1);
            else
            {
                error_message(argv[1], *(argv + i), "' needs a value.\n");
                exit(1);
            }
            i++;
        }
        else if(!ft_strcmp(*(argv + i), "-k"))
        {
            ret.key_size = atox(&ret.key, *(argv + i + 1));
            if(ret.key_size == 0)
            {
                error_message(argv[1], *(argv + i), "' needs a hex value.\n");
                exit(1);
            }
            i++;
        }
        else if(!ft_strcmp(*(argv + i), "-s"))
        {
            size_t salt_size = atox(&ret.salt, *(argv + i + 1));
            if(salt_size == 0)
            {
                error_message(argv[1], *(argv + i), "' needs a hex value.\n");
                exit(1);
            }
            else if(salt_size < 8)
            {
                uint8_t* tmp = key_pad(ret.salt, salt_size, 8);
                free(ret.salt);
                ret.salt = tmp;
            }
            i++;   
        }
        else if(!ft_strcmp(*(argv + i), "-v"))
        {
            if(atox(&ret.iv, *(argv + i + 1)) == 0)
            {
                error_message(argv[1], *(argv + i), "' needs a hex value.\n");
                exit(1);
            }
            i++;
        }
        else if(!ft_strcmp(*(argv + i), "-e"))
            ret.options = (ret.options >> 1) << 1;
        else if(!ft_strcmp(*(argv + i), "-d"))
            ret.options |= 0b01;
        else if(!ft_strcmp(*(argv + i), "-a"))
            ret.options |= 0b10;
        else if(!ft_strcmp(*(argv + i), "-P"))
            ret.options |= 0b100000000;
        else
        {
            error_message(argv[1], *(argv + i), "' is an invalid option.\n\n");
            help(argv);
        }
        i++;
    }
    return(ret);
}

void des_options(char** argv, int argc)
    {
        t_des_options ask = read_options(argv, argc);
        char* buff = NULL;
        size_t size;
        int fd;
        uint64_t* des_out = NULL;
        char* password = NULL;
        bool salted = false;

        if(ask.in_file)
        {
            off_t len = input_file(ask.in_file, &buff);
            if(len == -1)
            {
                error_message(argv[1], ask.in_file, "': No such file or directory\n");
                free(des_out);
                free(buff);
                free(password);
                free(ask.key);
                free(ask.iv);
                free(ask.salt);
                return ;
            }
            size = (size_t)len;
            if(!ask.password && !ask.key)
            {
                ft_getpass(&password, ask.options & 1, argv[1]);
                ask.password = password;
            }
        }
        else
        {
            if(!ask.password && !ask.key)
            {
                ft_getpass(&password, ask.options & 1, argv[1]);
                ask.password = password;
            }
            size = read_stdin(&buff, false);
        }
        
        if((ask.options & 0b11) == 0b11)
        {
            size_t size_out;
            char* base64 = base64_decode((uint8_t*)buff, size, &size_out);
            free(buff);
            buff = base64;
            size = size_out;
            if(size == 0)
            {
                write(STDOUT_FILENO, "ft_ssl: ", 8);
                write(STDOUT_FILENO, argv[1], ft_strlen(argv[1]));
                write(STDOUT_FILENO, ": error reading input file\n", 28);
                free(des_out);
                free(buff);
                free(password);
                free(ask.key);
                free(ask.iv);
                free(ask.salt);
                return ;
            }
        }

        if(!ask.key)
        {
            uint8_t* DK;
            uint8_t* salt;
            size_t len;
            int key_size;
            
            len = ft_strlen(ask.password);
            if((ask.options & 1) == 0b01)
            {
                salt = parse_salted(&buff, size);
                size-=16;
                if(!ask.salt)
                    ask.salt = salt;
                else
                    free(salt);
            }
            if(!ask.salt)
                ask.salt = i64tov8(ft_urand());
            if(ask.options & 0b10000000)
                key_size = 32;
            else
                key_size = 16;
            DK = pbdkf2((uint8_t*)ask.password, len, ask.salt, 10000, key_size, sha256, 32);
            ask.key_size = key_size - 8;
            ask.key = (uint8_t*)malloc(ask.key_size);
            for (size_t i = 0; i < ask.key_size; i++)
                ask.key[i] = DK[i];
            if(!ask.iv)
            {
                ask.iv = (uint8_t*)malloc(8);
                for (size_t i = 0; i < 8; i++)
                    ask.iv[i] = DK[i + ask.key_size];
            }
            free(DK);
            salted = true;
        }
        if(!ask.iv)
            ask.iv = i64tov8(ft_urand());
        if(ask.options & 0b1000)
        {
            free(ask.iv);
            ask.iv = NULL;
        }
        if(ask.key_size < 24 && ask.options & 0b10000000)
        {
            uint8_t* tmp = key_pad(ask.key, ask.key_size, 24);
            free(ask.key);
            ask.key = tmp;
            ask.key_size = 24;
        }
        else if(ask.key_size < 8)
        {
            uint8_t* tmp = key_pad(ask.key, ask.key_size, 8);
            free(ask.key);
            ask.key = tmp;
            ask.key_size = 8;
        }
        
        if((ask.options & 1) == 0)
        {
            uint8_t* padded = NULL;
            int mode = ask.options & 0b01111100;
            int prop = 0;
            if(ask.options & 0b1000000000)
                prop++;
            if(ask.options & 0b10000000)
            {
                uint64_t k1 = *((uint64_t*)ask.key);
                uint64_t k2 = *((uint64_t*)(ask.key+8));
                uint64_t k3 = *((uint64_t*)(ask.key+16));
                uint64_t* des_in;
                if(mode & 0b1100)
                {
                    size = pkcss5_padding(&padded, (uint8_t*)buff, 8);
                    des_out = des(padded, size, (uint8_t*)&k1, ask.iv, DES_ENCRYPT + prop);
                    des_in = des_out;
                    des_out = des((uint8_t*)des_in, size, (uint8_t*)&k2, ask.iv, DES_DECRYPT + prop);
                    free(des_in);
                    des_in = des_out;
                    des_out = des((uint8_t*)des_in, size, (uint8_t*)&k3, ask.iv, DES_ENCRYPT + prop);
                    free(des_in);
                }
                else
                {
                    des_out = des_bonus((uint8_t*)buff, size, (uint8_t*)&k1, ask.iv, mode);
                    des_in = des_out;
                    mode |= 0b01;
                    des_out = des_bonus((uint8_t*)des_in, size, (uint8_t*)&k2, ask.iv, mode);
                    free(des_in);
                    des_in = des_out;
                    mode = (mode >> 1) << 1;
                    des_out = des_bonus((uint8_t*)des_in, size, (uint8_t*)&k3, ask.iv, mode);
                    free(des_in);
                }
            }
            else
            {
                if(mode & 0b1100)
                {
                    size = pkcss5_padding(&padded, (uint8_t*)buff, 8);
                    des_out = des(padded, size, ask.key, ask.iv, DES_ENCRYPT + prop);
                }
                else
                    des_out = des_bonus((uint8_t*)buff, size, ask.key, ask.iv, mode);
            }
            free(padded);
        }
        else
        {
            // if((ask.options & 0b11) == 0b01)
            // {
            //     uint8_t* vec = NULL;
            //     size = atox(&vec, buff);
            //     free(buff);
            //     buff = (char*)vec;
            // }
            // if(size % 8 != 0)
            // {
            //     write(2, "error", 5);
            //     exit(1);
            // }
            int mode = ask.options & 0b01111101;
            int prop = 0;
            if(ask.options & 0b1000000000)
                prop++;
            if(ask.options & 0b10000000)
            {
                uint64_t k1 = *((uint64_t*)ask.key);
                uint64_t k2 = *((uint64_t*)(ask.key+8));
                uint64_t k3 = *((uint64_t*)(ask.key+16));
                uint64_t* des_in;
                if(mode & 0b1100)
                {
                    des_out = des((uint8_t*)buff, size, (uint8_t*)&k3, ask.iv, DES_DECRYPT + prop);
                    des_in = des_out;
                    des_out = des((uint8_t*)des_in, size, (uint8_t*)&k2, ask.iv, DES_ENCRYPT + prop);
                    free(des_in);
                    des_in = des_out;
                    des_out = des((uint8_t*)des_in, size, (uint8_t*)&k1, ask.iv, DES_DECRYPT + prop);
                    free(des_in);
                    size -= pkcss5_remove_pad((uint8_t**)(&des_out), size);
                }
                else
                {
                    des_out = des_bonus((uint8_t*)buff, size, (uint8_t*)&k3, ask.iv, mode);
                    des_in = des_out;
                    mode = (mode >> 1) << 1;
                    des_out = des_bonus((uint8_t*)des_in, size, (uint8_t*)&k2, ask.iv, mode);
                    free(des_in);
                    des_in = des_out;
                    mode |= 0b01;
                    des_out = des_bonus((uint8_t*)des_in, size, (uint8_t*)&k1, ask.iv, mode);
                    free(des_in);

                }
            }
            else
            {
                if(mode & 0b1100)
                {
                    des_out = des((uint8_t*)buff, size, ask.key, ask.iv, DES_DECRYPT + prop);
                    size -= pkcss5_remove_pad((uint8_t**)(&des_out), size);
                }
                else
                    des_out = des_bonus((uint8_t*)buff, size, ask.key, ask.iv, mode);
            }
        }
        if(ask.options & 0b100000000)
        {
            if(ask.salt)
            {
                write(STDOUT_FILENO, "salt=", 5);
                print_hash(STDOUT_FILENO, ask.salt, 64);
                write(STDOUT_FILENO, "\n", 1);
            }
            write(STDOUT_FILENO, "key=", 4);
            print_hash(STDOUT_FILENO, ask.key, ask.key_size * 8);
            write(STDOUT_FILENO, "\n", 1);
            if(ask.iv)
            {
                write(STDOUT_FILENO, "iv=", 3);
                print_hash(STDOUT_FILENO, ask.iv, 64);
                write(STDOUT_FILENO, "\n", 1);
            }
        }
        if(ask.out_file)
            fd = open(ask.out_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
        else
            fd = STDOUT_FILENO;
        // if((ask.options & 0b11)== 0b00)
        //     print_hash(fd, (uint8_t*)des_out, size * 8);
        if((ask.options & 0b11) == 0b10)
        {
            uint8_t* vec = hash_to_vec(des_out, size);
            if(size % 8 == 0)
                size = sizeof(uint64_t) * (size / 8);
            if(salted)
            {
                uint8_t salt_head[8] = "Salted__";
                uint8_t* tmp = (uint8_t*)malloc(size + 16);
                for (size_t i = 0; i < 8; i++)
                    tmp[i] = salt_head[i];
                for (size_t i = 0; i < 8; i++)
                    tmp[i+8] = ask.salt[i];
                for (size_t i = 0; i < size; i++)
                    tmp[i+16] = vec[i];
                free(vec);
                vec = tmp;
                size += 16;
            }
            char* cypher = base64_encode(vec, size);
            print_base64(fd, cypher, ft_strlen(cypher));
            free(vec);
            free(cypher);
        }
        else
        {
            if((ask.options & 1) != 1 && salted)
            {
                char salt_head[8] = "Salted__";
                write(fd, salt_head, 8);
                write(fd, ask.salt, 8);
            }
            write(fd, des_out, size);
        }
        if(fd != STDOUT_FILENO)
            close(fd);

        free(des_out);
        free(buff);
        free(password);
        free(ask.key);
        free(ask.iv);
        free(ask.salt);
    }