#include <ft_ssl.h>

uint8_t* hash_to_vec(uint64_t* hash, size_t h_bytes)
{
    size_t vec_size = sizeof(uint64_t) * (h_bytes / 8);
    uint8_t* vec = (uint8_t*)malloc(vec_size + 1);
    size_t cursor = 0;

    while(cursor < h_bytes)
    {
        for (size_t i = 0; i < 8; i++)
        {
            vec[cursor + 7 - i] = 0;
            vec[cursor + 7 - i] |= *hash >> (64 - (8 * (i+1)));
        }
        hash++;
        cursor += 8;
    }
    vec[vec_size] = '\0';
    return(vec);
}