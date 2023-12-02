#include <ft_ssl.h>

size_t parse_pem(char** pem, char* buff, size_t size, int vis, bool* salted, char** iv)
{
    char* body;
    char* enc;
    char* footer;
    char* c_foot;
    char* c_head;
    char* c_err;
    size_t head_size;
    size_t foot_size;
    size_t enc_size;
    size_t err_size;
    size_t pem_size;
    size_t ln_count;
    if(vis == PRIVATE)
    {
        c_head = (char*)"-----BEGIN RSA PRIVATE KEY-----\n";
        c_foot = (char*)"-----END RSA PRIVATE KEY-----";
        c_err = (char*)"Not a RSA PRIVATE KEY file\n";
        head_size = 32;
        foot_size = 29;
        err_size = 27;
    }
    else if(vis == DSA)
    {
        c_head = (char*)"-----BEGIN DSA PRIVATE KEY-----\n";
        c_foot = (char*)"-----END DSA PRIVATE KEY-----";
        c_err = (char*)"Not a DSA PRIVATE KEY file\n";
        head_size = 32;
        foot_size = 29;
        err_size = 27;
    }
    else
    {
        c_head = (char*)"-----BEGIN PUBLIC KEY-----\n";
        c_foot = (char*)"-----END PUBLIC KEY-----";
        c_err = (char*)"Not a RSA PUBLIC KEY file\n";
        head_size = 27;
        foot_size = 24;
        err_size = 26;
    }

    body = buff + head_size;
    enc_size = 0;
    ln_count = 0;

    int is_head = ft_strlcmp(buff, c_head, head_size);

    if(is_head == 0)
    {
        if(!ft_strlcmp(body, "Proc-Type: 4,ENCRYPTED\n", 23))
        {
            enc = body;
            body += 23;

            if(ft_strlcmp(body, "DEK-Info: DES-CBC", 17))
            {
                write(1, c_err, err_size);
                return(0);
            }
            body += 17;
            
            if(ft_strlcmp(body, ",", 1))
            {
                write(1, c_err, err_size);
                return(0);
            }
            body+=1;

            int i = 0;
            while (*body != '\n')
            {
                body++;
                i++;
            }
            body -= i;
            *iv = (char*)malloc(i+1);
            for (int j = 0; j < i; j++)
                (*iv)[j] = body[j];
            (*iv)[i] = 0;
            body += i + 2;
            enc_size = body - enc;
            *salted = true;
        }
        while (ft_strlcmp(body, c_foot, foot_size))
        {
            while(body != (buff + size))
            {
                if(*body++ == '\n')
                {
                    ln_count++;
                    break;
                }
            }
            if(body == (buff + size))
            {
                write(1, c_err, err_size);
                return(0);
            }
        }
        footer = body;
        pem_size = footer - buff - head_size - enc_size - ln_count;
        body = (char*)malloc(pem_size + 1);
        buff += head_size + enc_size;

        for (size_t i = 0; i < pem_size; i++)
        {
            if(*buff == '\n')
                buff++;
            body[i] = *buff;
            buff++;
        }
        body[pem_size] = '\0';
    }
    else
    {
        write(1, c_err, err_size);
        return(0);
    }
    *pem = body;
	return(pem_size);
}