
#include <ft_ssl.h>

t_der* decode_pem(char* pem, size_t len, int mode)
{
    char* text;
    t_der* der;
    

    der = (t_der*)malloc(sizeof(t_der));
    text = base64_decode((uint8_t*)pem, len, &der->size);
    parse_der(der, (uint8_t*)text, der->size, mode);
    free(text);

    return(der);
}