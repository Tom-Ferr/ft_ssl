#include <ft_ssl.h>

static uint32_t s0(uint32_t w)
{
    return(right_rotation(w, 7) ^ right_rotation(w, 18) ^ (w >> 3));
}

static uint32_t s1(uint32_t w)
{
    return(right_rotation(w, 17) ^ right_rotation(w, 19) ^ (w >> 10));
}

static uint32_t* append_blocks(uint32_t* blocks, uint64_t len)
{
    uint32_t* w;
    uint32_t* w_0;
    uint32_t* blocks_0;
    int i;
    uint64_t n_chunks;

    n_chunks = (len * 8) / 512;
    w = (uint32_t*)malloc((64 * n_chunks) * sizeof(uint32_t));
    if (!w)
        return(NULL);
    
    w_0 = w;
    blocks_0 = blocks;
    while(n_chunks)
    {
        i = 0;
        while (i < 64)
        {
            if (i < 16){

                w[i] = ft_bswap32(blocks[i]);
            }
            else
                w[i] = 0x0;
            i++;
        }

        i = 16;
        while (i < 64)
        {
            w[i] = (w[i - 16] + s0(w[i - 15])) % L; 
            w[i] = (w[i] + w[i - 7]) % L; 
            w[i] = (w[i] + s1(w[i - 2])) % L;
            i++; 
        }
        n_chunks--;
        blocks += 16;
        w += 64;
    }
    free(blocks_0);
    return(w_0);
}

static uint32_t* init_buffers(void)
{
    uint32_t* buffers;

    buffers = (uint32_t*)malloc(8 * sizeof(uint32_t));
    if(!buffers)
        return(NULL);

    buffers[0] = 0xc1059ed8;
    buffers[1] = 0x367cd507;
    buffers[2] = 0x3070dd17;
    buffers[3] = 0xf70e5939;
    buffers[4] = 0xffc00b31;
    buffers[5] = 0x68581511;
    buffers[6] = 0x64f98fa7;
    buffers[7] = 0xbefa4fa4;

    return(buffers);

}

static uint8_t* digest(uint32_t* blocks, uint32_t* buffer, uint64_t n_chunks)
{
    int i;
    uint32_t result_1;
    uint32_t result_2;
    uint32_t k[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
        0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
        0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
        0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
        0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
        0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };
    uint32_t original[8];

    while(n_chunks)
    {
        copy_buffer(buffer, original, 8);
        i = 0;
        while(i < 64)
        {
            result_1 = (k[i] + blocks[i]) % L;
            result_1 = (result_1 + ch(buffer[4], buffer[5], buffer[6])) % L;
            result_1 = (result_1 + sigma1(buffer[4])) % L;
            result_1 = (result_1 + buffer[7]) % L;
            buffer[3] = (buffer[3] + result_1) % L;
            result_2 = (sigma2(buffer[0]) + maj(buffer[0], buffer[1], buffer[2])) % L;
            update_buffer(buffer, result_2, 8,0);
            buffer[0] = (buffer[0] + result_1) % L;
            i++;
        }
        add_buffers(buffer, original, 8);
        blocks += 64;
        n_chunks--;
    }


    i = 0;
    while (i < 8)
    {
        buffer[i] = ft_bswap32(buffer[i]);
        i++;
    }

    int hash_len = 224/8;
    uint8_t* hash = (uint8_t*)malloc(hash_len + 1);
    for (int i = 0; i < hash_len; i++)
        hash[i] = *((uint8_t*)buffer + i);
    hash[hash_len] = '\0';
    return (hash);
}

uint8_t* sha224(uint8_t* argv, uint64_t len)
{
    uint64_t* padded_text;
    uint32_t* buffer;
    uint32_t* blocks;
    uint8_t* hash;
    
    padded_text = add_padding(argv, &len, 64, 8);

    if(!padded_text)
        return(NULL);
    bswap_length_field(padded_text + ((len - 8) / 8), 8);
    buffer = init_buffers();
    if(!buffer)
        return(NULL);
    blocks = divide_into_blocks(padded_text);
    blocks = append_blocks(blocks, len);

    hash = digest(blocks, buffer,  (len * 8) / 512);

    free(blocks);
    free(buffer);
    return(hash);
}