
#include <ft_ssl.h>

static char* build_base()
{
    char* base;
    char init;
    int i;

    base = (char*)malloc(64);
    if(!base)
        return(NULL);
    
    i=0;
    init = 'A';
    while(i < 64)
    {
        base[i] = init;
        if(init == 'Z')
            init = 'a';
        else if(init == 'z')
            init = '0';
        else if(init == '9')
            init = '+';
        else if(init == '+')
            init = '/';
        else
            init++;
        i++;
    }
    return(base);

}

static void split_6bit(uint8_t* plain, char* base, char* cypher, size_t len)
{
    size_t i;
    size_t j;
    int n;
    int rest;

    i = 0;
    j = 0;
    rest = 0;
    while(i < len)
    {
        n = rest << ((4 * (i % 3)) % 6);
        n = n | (plain[i] >> ((1 + (i % 3)) * 2));
        cypher[j] = base[n];
        rest = plain[i] % (2 << ((2 * (i % 3)) + 1));
        if((i+1) % 3 == 0)
        {
            j++;
            cypher[j] = base[rest];
            rest = 0;
        }
        i++;
        j++;
    }
    if(j % 4)
    {
        rest = rest << ((4 * (i % 3)) % 6);
        cypher[j] = base[rest];
        j++;
    }
    while(i % 3)
    {
        cypher[j] = '=';
        i++;
        j++;
    }
    cypher[j] = '\0';
}

static char* encode(uint8_t* plain, char* base, size_t len)
{
    char* cypher;
    size_t cypher_len;
    
    cypher_len = ((len / 3) + !(!(len % 3))) * 4;

    cypher = (char*)malloc(cypher_len + 1);
    if(!cypher)
        return(NULL);
    
    split_6bit(plain, base, cypher, len);
    
    return(cypher);
    
}

char* base64_encode(uint8_t* argv, size_t len)
{
    char* base;
    char* cypher;

    base = build_base();
    cypher = encode(argv, base, len);
    free(base);
    return(cypher);
}