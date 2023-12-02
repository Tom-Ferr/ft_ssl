#include <ft_ssl.h>

void print_rsakey(int fd, char* text, int vis, bool salted, char* iv) 
{
    char pv[16] = "RSA PRIVATE KEY";
    char pb[11] = "PUBLIC KEY";
    char dsa[16] = "DSA PRIVATE KEY";

    write(fd, "-----BEGIN ", 11);
    if(vis == PRIVATE)
        write(fd, pv, 15);
    else if(vis == DSA)
        write(fd, dsa, 15);
    else
       write(fd, pb, 10); 
    write(fd, "-----\n", 6);
    if(salted)
    {
        write(fd, "Proc-Type: 4,ENCRYPTED\n", 23);
        write(fd, "DEK-Info: ", 10);
        write(fd, "DES-CBC", 7);
        write(fd, ",", 1);
        write(fd, iv, ft_strlen(iv));
        write(fd, "\n\n", 2);
    }
    print_base64(fd, text, ft_strlen(text));
    write(fd, "-----END ", 9);
    if(vis == PRIVATE)
        write(fd, pv, 15);
    else if(vis == DSA)
        write(fd, dsa, 15);
    else
       write(fd, pb, 10);
    write(fd, "-----\n", 6);
}

void print_eval(int fd, t_bigint* e)
{
    print_bigint(fd, e, 10);
    write(fd, " (0x", 4);
    print_bigint(fd, e, 16);
    write(fd, ")\n", 2);
}
