
#include <ft_ssl.h>

void copy_buffer(uint32_t* buffer, uint32_t* original, int size)
{
    int i;

    i = 0;
    while(i < size)
    {
        original[i] = buffer[i];
        i++;
    }
}

void add_buffers(uint32_t* buffer, uint32_t* original, int size)
{
    int i;

    i = 0;
    while(i < size)
    {
        buffer[i] = (buffer[i] + original[i]) % L;
        i++;
    }
}

void update_buffer(uint32_t* buffer, uint32_t result, int size, int h)
{
    int i;

    i = size;
    while(i > h)
    {
        if(i % size != h)
            buffer[i % size] = buffer[(i - 1) % size];
        i--;
    }
    buffer[h] = result;
}
void copy_buffer_64(uint64_t* buffer, uint64_t* original, int size)
{
    int i;

    i = 0;
    while(i < size)
    {
        original[i] = buffer[i];
        i++;
    }
}

void add_buffers_64(uint64_t* buffer, uint64_t* original, int size)
{
    int i;

    i = 0;
    while(i < size)
    {
        buffer[i] = addition_modulus_uint64max(buffer[i], original[i]);
        i++;
    }
}

void update_buffer_64(uint64_t* buffer, uint64_t result, int size, int h)
{
    int i;

    i = size;
    while(i > h)
    {
        if(i % size != h)
            buffer[i % size] = buffer[(i - 1) % size];
        i--;
    }
    buffer[h] = result;
}


