#include <ft_ssl.h>

uint8_t* mgf1(uint8_t* Z, size_t z_bytes, size_t t_bytes, uint8_t* (*H)(uint8_t*, uint64_t), size_t h_bytes)
{
    uint8_t* t;
    uint8_t* u;
    uint8_t* hash;
    size_t u_bytes;

    if(t_bytes > (h_bytes << 32))
        return(NULL);
    
    t = (uint8_t*)malloc(t_bytes);
    if(!t)
        return(NULL);

    u_bytes = z_bytes + 4;

    for (size_t i = 0; i < t_bytes; i += h_bytes)
    {
        u = (uint8_t*)malloc(u_bytes);

        /*
        Z || i32
        */
        for (size_t j = 0; j < z_bytes; j++)
            u[j] = Z[j];

        uint32_t i32 = (uint32_t)i;
        for (size_t l = 0; l < 4; l++)
            u[z_bytes + l] = (uint8_t)(i32 >> (32 - (8 * (l+1))));
        
        hash = H(u, u_bytes);
        
        /*
        T || H(Z || i32)
        */
        for (size_t j = 0; (i + j) < t_bytes && j < h_bytes; j++)
            t[i + j] = hash[j];
        
        free(hash);
        free(u);
    }

    return(t);
}