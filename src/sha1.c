#include <ft_ssl.h>

static uint32_t* append_blocks(uint32_t* blocks, uint64_t len)
{
    uint32_t* w;
    uint32_t* w_0;
    uint32_t* blocks_0;
    int i;
    uint64_t n_chunks;

    n_chunks = (len * 8) / 512;
    w = (uint32_t*)malloc((80 * n_chunks) * sizeof(uint32_t));
    if (!w)
        return(NULL);
    
    w_0 = w;
    blocks_0 = blocks;
    while(n_chunks)
    {
        i = 0;
        while (i < 80)
        {
            if (i < 16){

                w[i] = ft_bswap32(blocks[i]);
            }
            else
                w[i] = 0x0;
            i++;
        }

        i = 16;
        while (i < 80)
        {
            w[i] = left_rotation(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);
            i++; 
        }
        n_chunks--;
        blocks += 16;
        w += 80;
    }
    free(blocks_0);
    return(w_0);
}

static uint32_t* init_buffers(void)
{
    uint32_t* buffers;

    buffers = (uint32_t*)malloc(5 * sizeof(uint32_t));
    if(!buffers)
        return(NULL);

    buffers[0] = 0x67452301;
    buffers[1] = 0xefcdab89;
    buffers[2] = 0x98badcfe;
    buffers[3] = 0x10325476;
    buffers[4] = 0xc3d2e1f0;

    return(buffers);

}

static uint8_t* digest(uint32_t* blocks, uint32_t* buffer, uint64_t n_chunks)
{
    int i;
    uint32_t result_1;
    uint32_t k[4] = {0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6};
    uint32_t original[5];
    t_nlp procs[4];

    procs[0] = &ch;
    procs[1] = &h_process;
    procs[2] = &maj;
    procs[3] = &h_process;

    while(n_chunks)
    {
        copy_buffer(buffer, original, 5);
        i = 0;
        while(i < 80)
        {
            result_1 = (left_rotation(buffer[0], 5) + procs[i / 20](buffer[1], buffer[2], buffer[3])) % L;
            result_1 = (result_1 + buffer[4]) % L;
            result_1 = (result_1 + k[i / 20]) % L;
            result_1 = (result_1 + blocks[i]) % L;
            update_buffer(buffer, result_1, 5,0);
            buffer[2] = left_rotation(buffer[2], 30);
            i++;
        }
        add_buffers(buffer, original, 5);
        blocks += 80;
        n_chunks--;
    }


    i = 0;
    while (i < 5)
    {
        buffer[i] = ft_bswap32(buffer[i]);
        i++;
    }

    int hash_len = 160/8;
    uint8_t* hash = (uint8_t*)malloc(hash_len + 1);
    for (int i = 0; i < hash_len; i++)
        hash[i] = *((uint8_t*)buffer + i);
    hash[hash_len] = '\0';
    return (hash);
}

uint8_t* sha1(uint8_t* argv, uint64_t len)
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