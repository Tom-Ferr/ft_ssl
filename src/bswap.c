#include <ft_ssl.h>

uint64_t ft_bswap64(uint64_t value) {
    uint64_t result = ((value & 0x00000000000000FFUL) << 56) |
                      ((value & 0x000000000000FF00UL) << 40) |
                      ((value & 0x0000000000FF0000UL) << 24) |
                      ((value & 0x00000000FF000000UL) << 8) |
                      ((value & 0x000000FF00000000UL) >> 8) |
                      ((value & 0x0000FF0000000000UL) >> 24) |
                      ((value & 0x00FF000000000000UL) >> 40) |
                      ((value & 0xFF00000000000000UL) >> 56);
    return(result);
}

uint32_t ft_bswap32(uint32_t value) {
    uint32_t result = ((value & 0x000000FFU) << 24) |
                      ((value & 0x0000FF00U) << 8) |
                      ((value & 0x00FF0000U) >> 8) |
                      ((value & 0xFF000000U) >> 24);
    return(result);
}

void bswap_length_field(uint64_t* text, size_t length_field)
{
    size_t mod = length_field / 8;
    uint64_t* buff = (uint64_t*)malloc(sizeof(uint64_t) * mod);


    for(size_t i = 0; i < mod; i++)
        buff[i] = ft_bswap64(text[i]);
    for(size_t i = 0; i < mod; i++)
        text[i] = buff[mod - i - 1];
    free(buff);
}