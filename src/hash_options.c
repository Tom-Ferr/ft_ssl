#include <ft_ssl.h>

static void help(char** argv)
{
    write(STDOUT_FILENO, "Usage: ", 7);
    write(STDOUT_FILENO, argv[1], ft_strlen(argv[1]));
    write(STDOUT_FILENO, " [options] [file...]\nValid options are:\n", 40);

    write(STDOUT_FILENO, "-help\t\tDisplay this summary\n", 28);
    write(STDOUT_FILENO, "-p\t\tEcho STDIN to STDOUT and append the checksum to STDOUT\n", 59);
    write(STDOUT_FILENO, "-q\t\tQuiet mode\n", 15);
    write(STDOUT_FILENO, "-r\t\tReverse the format of the output\n", 37);
    write(STDOUT_FILENO, "-s\t\tPrint the sum of the given string\n", 38);
    exit(0);
}

static t_hash_options read_options(char* argv[], int argc)
{
    int i = 2;
    t_hash_options ret;
    ret.string = NULL;
    ret.file = NULL;
    ret.options = 0;

    while(i < argc)
    {
        if(!ft_strcmp(*(argv + i), "-help") && ret.file == NULL)
            help(argv);
        else if(!ft_strcmp(*(argv + i), "-p") && ret.file == NULL)
            ret.options |= 0b1000;
        else if(!ft_strcmp(*(argv + i), "-q") && ret.file == NULL)
            ret.options |= 0b0100;
        else if(!ft_strcmp(*(argv + i), "-r") && ret.file == NULL)
            ret.options |= 0b0010;
        else if(!ft_strcmp(*(argv + i), "-s") && ret.file == NULL)
        {
            if(ret.string == NULL)
            {
                ret.options |= 0b0001;
                if(*(argv + i+1))
                    ret.string = *(argv + i+1);
                else
                {
                    error_message(argv[1], *(argv + i), "' needs a value.\n");
                    exit(1);
                }
            }
            i += 1;
        }
        else if(**(argv + i) != '-' && ret.file == NULL)
            ret.file = argv + i;
        else if(ret.file == NULL)
        {
            error_message(argv[1], *(argv + i), "' is an invalid option.\n\n");
            help(argv);
        }
        i++;
    }
    return(ret);
}

void hash_options(uint8_t* H(uint8_t*, uint64_t), size_t h_bit, char** argv, int argc)
    {
        char* H_str;
        char* h_str;
        t_hash_options ask = read_options(argv, argc);
        if(H == md5)
        {
            H_str = "MD5";
            h_str = "md5";
        }
        else if (H == sha1)
        {
            H_str = "SHA1";
            h_str = "sha1";
        }
        else if (H == sha224)
        {
            H_str = "SHA224";
            h_str = "sha224";
        }
        else if (H == sha256)
        {
            H_str = "SHA256";
            h_str = "sha256";
        }
        else if (H == sha384)
        {
            H_str = "SHA384";
            h_str = "sha384";
        }
        else if (H == sha512)
        {
            H_str = "SHA512";
            h_str = "sha512";
        }
        else if (H == sha512_224)
        {
            H_str = "SHA512-224";
            h_str = "sha512-224";
        }
        else if (H == sha512_256)
        {
            H_str = "SHA512-256";
            h_str = "sha512-256";
        }
        else if (H == whirlpool)
        {
            H_str = "WHIRLPOOL";
            h_str = "whirlpool";
        }
        if((ask.file == NULL && (ask.options & 1) == 0) || (ask.options & 0b1000) == 0b1000)
        {
            char* buff;
            size_t size = read_stdin(&buff, false);
            uint8_t* hash = H((uint8_t*)buff, size);
            // if((ask.options & 0b0110) == 0)
            {
                if((ask.options & 0b1110) == 0)
                {
                    // write(STDOUT_FILENO, H_str, ft_strlen(H_str));
                    write(STDOUT_FILENO, "(stdin)= ", 9);
                }
                else if((ask.options & 0b1000) == 0b1000)
                {
                    if((ask.options & 0b0100) == 0)
                        write(STDOUT_FILENO, "(\"", 2);
                    if(buff[size-1] == '\n')
                        size--;
                    write(STDOUT_FILENO, buff, size);
                    if((ask.options & 0b0100) == 0b0100)
                        write(STDOUT_FILENO, "\n", 1);
                    else
                        write(STDOUT_FILENO, "\")= ", 4);
                }
            }
            print_hash(STDOUT_FILENO, hash, h_bit);
            if((ask.options & 0b1110) == 0b0010)
            {
                write(STDOUT_FILENO, " ", 1);
                write(STDOUT_FILENO, "*stdin", 6);
            }
            // else if((ask.options & 0b1110) == 0b1010)
            // {
            //     write(STDOUT_FILENO, " \"", 2);
            //     if(buff[size-1] == '\n')
            //         size--;
            //     write(STDOUT_FILENO, buff, size);
            //     write(STDOUT_FILENO, "\"", 1);
            // }
            write(STDOUT_FILENO, "\n", 1);
            free(hash);
            free(buff);
        }
        if(ask.options & 1)
        {
            uint8_t* hash = H((uint8_t*)ask.string, ft_strlen(ask.string));
            if((ask.options & 0b0100) == 0)
            {
                if((ask.options & 0b0110) == 0)
                {
                    write(STDOUT_FILENO, H_str, ft_strlen(H_str));
                    write(STDOUT_FILENO, " (\"", 3);
                    write(STDOUT_FILENO, ask.string, ft_strlen(ask.string));
                    write(STDOUT_FILENO, "\") = ", 5);
                }
            }
            print_hash(STDOUT_FILENO, hash, h_bit);
            if((ask.options & 0b0110) == 0b0010)
            {
                write(STDOUT_FILENO, " \"", 2);
                write(STDOUT_FILENO, ask.string, ft_strlen(ask.string));
                write(STDOUT_FILENO, "\"", 1);
            }
            write(STDOUT_FILENO, "\n", 1);
            free(hash);
        }
        if(ask.file)
        {
            while(*ask.file)
            {
                char* buff;
                off_t size = input_file(*ask.file, &buff);
                if(size == -1)
                {
                    write(STDOUT_FILENO, "ft_ssl: ", 8);
                    write(STDOUT_FILENO, h_str, ft_strlen(h_str));
                    write(STDOUT_FILENO, ": ", 2);
                    write(STDOUT_FILENO, *ask.file, ft_strlen(*ask.file));
                    write(STDOUT_FILENO, ": No such file or directory\n", 28);
                    ask.file++;
                    continue;
                }
                uint8_t* hash = H((uint8_t*)buff, size);
                if((ask.options & 0b0100) == 0)
                {
                    if((ask.options & 0b0110) == 0)
                    {
                        write(STDOUT_FILENO, H_str, ft_strlen(H_str));
                        write(STDOUT_FILENO, " (", 2);
                        write(STDOUT_FILENO, *ask.file, ft_strlen(*ask.file));
                        write(STDOUT_FILENO, ") = ", 4);
                    }
                }
                print_hash(STDOUT_FILENO, hash, h_bit);
                if((ask.options & 0b0110) == 0b0010)
                {
                    write(STDOUT_FILENO, " ", 1);
                    write(STDOUT_FILENO, *ask.file, ft_strlen(*ask.file));
                }
                write(STDOUT_FILENO, "\n", 1);
                free(hash);
                free(buff);
                ask.file++;
            }
        }
    }