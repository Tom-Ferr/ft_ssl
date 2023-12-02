
#include <ft_ssl.h>

static void help(void)
{
    write(STDOUT_FILENO, "\nStandard commands:\n", 20);
    write(STDOUT_FILENO, "help\n", 5);
    write(STDOUT_FILENO, "genrsa\n", 7);
    write(STDOUT_FILENO, "rsa\n", 4);
    write(STDOUT_FILENO, "rsautl\n", 7);
    write(STDOUT_FILENO, "gendsa\n", 7);
    write(STDOUT_FILENO, "\nMessage Digest commands:\n", 26);
    write(STDOUT_FILENO, "md5\n", 5);
    write(STDOUT_FILENO, "sha1\n", 5);
    write(STDOUT_FILENO, "sha224\n", 7);
    write(STDOUT_FILENO, "sha256\n", 7);
    write(STDOUT_FILENO, "sha384\n", 7);
    write(STDOUT_FILENO, "sha512\n", 7);
    write(STDOUT_FILENO, "sha512-224\n", 11);
    write(STDOUT_FILENO, "sha512-256\n", 11);
    write(STDOUT_FILENO, "whirlpool\n", 10);
    write(STDOUT_FILENO, "\nCipher commands:\n", 18);
    write(STDOUT_FILENO, "des\t\t\t", 6);
    write(STDOUT_FILENO, "des3\t\t\t", 7);
    write(STDOUT_FILENO, "aes-128\n", 8);
    write(STDOUT_FILENO, "des-ecb\t\t\t", 10);
    write(STDOUT_FILENO, "des3-ecb\t\t", 10);
    write(STDOUT_FILENO, "aes-128-ecb\n", 12);
    write(STDOUT_FILENO, "des-cbc\t\t\t", 10);
    write(STDOUT_FILENO, "des3-cbc\t\t", 10);
    write(STDOUT_FILENO, "aes-128-cbc\n", 12);
    write(STDOUT_FILENO, "des-cfb\t\t\t", 10);
    write(STDOUT_FILENO, "des3-cfb\t\t", 10);
    write(STDOUT_FILENO, "aes-128-cfb\n", 12);
    write(STDOUT_FILENO, "des-ofb\t\t\t", 10);
    write(STDOUT_FILENO, "des3-ofb\t\t", 10);
    write(STDOUT_FILENO, "aes-128-ofb\n", 12);
    write(STDOUT_FILENO, "des-ctr\t\t\t", 10);
    write(STDOUT_FILENO, "des3-ctr\t\t", 10);
    write(STDOUT_FILENO, "aes-128-ctr\n", 12);
    write(STDOUT_FILENO, "des-pcbc\t\t", 10);
    write(STDOUT_FILENO, "des3-pcbc\t\t", 11);
    write(STDOUT_FILENO, "aes-128-pcbc\n", 13);
    write(STDOUT_FILENO, "base64\n", 7);
    exit(0);
}

int main(int argc, char* argv[]){

    char** cmds;
    char* in;
    char* cmd;
    int ctr = 0;
    if(argc < 2)
    {
        write(1, "ft_ssl > ", 9);
        size_t size = read_stdin(&in, true);
        cmd = format_cmd(in, size);
        cmds = ft_split(cmd, ' ');
        free(in);
        free(cmd);
        for (char** i = cmds; *i; i++)
            ctr++;
    }
    else
    {
        cmds = argv;
        ctr = argc;
    }

    if(!ft_strcmp(cmds[1], "md5"))
        hash_options(md5, 128, cmds, ctr);
    else if(!ft_strcmp(cmds[1], "sha1"))
        hash_options(sha1, 160, cmds, ctr);
    else if(!ft_strcmp(cmds[1], "sha224"))
        hash_options(sha224, 224, cmds, ctr);
    else if(!ft_strcmp(cmds[1], "sha256"))
        hash_options(sha256, 256, cmds, ctr);
    else if(!ft_strcmp(cmds[1], "sha384"))
        hash_options(sha384, 384, cmds, ctr);
    else if(!ft_strcmp(cmds[1], "sha512"))
        hash_options(sha512, 512, cmds, ctr);
    else if(!ft_strcmp(cmds[1], "sha512-224"))
        hash_options(sha512_224, 224, cmds, ctr);
    else if(!ft_strcmp(cmds[1], "sha512-256"))
        hash_options(sha512_256, 256, cmds, ctr);
    else if(!ft_strcmp(cmds[1], "whirlpool"))
        hash_options(whirlpool, 512, cmds, ctr);
    else if(!ft_strcmp(cmds[1], "base64"))
        base64_options(cmds, ctr);
    else if(!ft_strcmp(cmds[1], "des")
    || !ft_strcmp(cmds[1], "des-cbc")
    || !ft_strcmp(cmds[1], "des-ecb")
    || !ft_strcmp(cmds[1], "des-cfb")
    || !ft_strcmp(cmds[1], "des-ofb")
    || !ft_strcmp(cmds[1], "des-pcbc")
    || !ft_strcmp(cmds[1], "des-ctr")
    || !ft_strcmp(cmds[1], "des3")
    || !ft_strcmp(cmds[1], "des3-cbc")
    || !ft_strcmp(cmds[1], "des3-ecb")
    || !ft_strcmp(cmds[1], "des3-cfb")
    || !ft_strcmp(cmds[1], "des3-ofb") 
    || !ft_strcmp(cmds[1], "des3-pcbc")
    || !ft_strcmp(cmds[1], "des3-ctr"))
        des_options(cmds, ctr);
    else if(!ft_strcmp(cmds[1], "genrsa"))
        genrsa_options(cmds, ctr);
    else if(!ft_strcmp(cmds[1], "gendes"))
        gendes_options(cmds, ctr);
    else if(!ft_strcmp(cmds[1], "gendsa"))
        gendsa_options(cmds, ctr);
    else if(!ft_strcmp(cmds[1], "rsa"))
        rsa_options(cmds, ctr);
    else if(!ft_strcmp(cmds[1], "dsa"))
        dsa_options(cmds, ctr);
    else if(!ft_strcmp(cmds[1], "rsautl"))
        rsautl_options(cmds, ctr);
    else if(!ft_strcmp(cmds[1], "aes-128")
    || !ft_strcmp(cmds[1], "aes-128-cbc")
    || !ft_strcmp(cmds[1], "aes-128-ecb")
    || !ft_strcmp(cmds[1], "aes-128-cfb")
    || !ft_strcmp(cmds[1], "aes-128-ofb")
    || !ft_strcmp(cmds[1], "aes-128-pcbc")
    || !ft_strcmp(cmds[1], "aes-128-ctr"))
        aes_128_options(cmds, ctr);
    else if(!ft_strcmp(cmds[1], "help"))
    {
        if(argc < 2)
            free_star_char(cmds);
        help();
    }
    else
    {
        write(STDOUT_FILENO, "ft_ssl: Error: '", 16);
        write(STDOUT_FILENO, cmds[1], ft_strlen(cmds[1]));
        write(STDOUT_FILENO, "' is an invalid command.\n", 25);
        write(STDOUT_FILENO, "Please, check possible commands by typing \"help\"\n", 49);
        
    }
    if(argc < 2)
        free_star_char(cmds);

    return(0);
}