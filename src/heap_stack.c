#include <ft_ssl.h>

uint64_t heap8_stack64(uint8_t* heap)
{
    if(!heap)
        return(0);
    uint64_t stack = 0;
    for(int i = 0; i < 8; i++)
    {
        stack <<= 8;
        stack |= heap[i];
    }
    return(stack);
}
uint32_t heap8_stack32(uint8_t* heap)
{
    if(!heap)
        return(0);
    uint32_t stack = 0;
    for(int i = 0; i < 4; i++)
    {
        stack <<= 8;
        stack |= heap[i];
    }
    return(stack);
}

uint8_t* i64tov8(uint64_t stack)
{
    uint8_t* heap = (uint8_t*)malloc(8);
    for (size_t i = 0; i < 8; i++)
    {
        heap[i] = 0;
        heap[i] |= stack >> (64 - (8 * (i+1)));
    }
    return(heap);
    
}