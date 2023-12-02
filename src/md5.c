
#include <ft_ssl.h>

static uint32_t* init_buffers(void)
{
    uint32_t* buffers;

    buffers = (uint32_t*)malloc(4 * sizeof(uint32_t*));
    if(!buffers)
        return(NULL);

    buffers[0] = 0x67452301;
    buffers[1] = 0xefcdab89;
    buffers[2] = 0x98badcfe;
    buffers[3] = 0x10325476;

    return(buffers);

}

static void process_loop(uint32_t* blocks, uint32_t* buffer,  t_nlp* procs)
{
    int r;
    int i;
    int j;
    uint32_t result;
    uint32_t** s;
    uint32_t** k;

    s = shift_array();
    k = constant_array();
    
    r = 0;
    while(r < 4)
    {
        i = 0;
        while( i < 16)
        {
            if(r)
                j = (r * 16 + i * ((16 + r) % 12 / ((r + 1) % 2 + 1)) + (r * 4 + 6) % 9) % 16;
            else
                j = i;
            result = procs[r](buffer[1], buffer[2], buffer[3]);
            result = (buffer[0] + result) % L;
            result = (blocks[j] + result) % L;
            result = (k[r][i] + result) % L;
            result = left_rotation(result, s[r][i]);
            result = (buffer[1] + result) % L;
            update_buffer(buffer, result,4, 1);
            i++;
        }
        r++;
    }
    free_star(s);
    free_star(k);
}

static uint8_t* digest(uint32_t* blocks, uint32_t* buffer, uint64_t n_chunk)
{
    uint32_t original[4];
    t_nlp procs[4];


    procs[0] = &f_process;
    procs[1] = &g_process;
    procs[2] = &h_process;
    procs[3] = &i_process;

    while(n_chunk)
    {
        copy_buffer(buffer, original, 4);
        process_loop(blocks, buffer, procs);
        add_buffers(buffer, original, 4);
        blocks += 16;
        n_chunk--;
    }

    int hash_len = 128/8;
    uint8_t* hash = (uint8_t*)malloc(hash_len + 1);
    for (int i = 0; i < hash_len; i++)
        hash[i] = *((uint8_t*)buffer + i);
    hash[hash_len] = '\0';
    return (hash);
}

uint8_t* md5(uint8_t* argv, uint64_t len)
{
    uint64_t* padded_text;
    uint8_t* hash;
    uint32_t* buffer;
    uint32_t* blocks;
    
    padded_text = add_padding(argv, &len, 64, 8);
    if(!padded_text)
        return(NULL);
    buffer = init_buffers();
    if(!buffer)
        return(NULL);
    blocks = divide_into_blocks(padded_text);
    hash = digest(blocks, buffer, (len * 8) / 512);

    free(padded_text);
    free(buffer);
    return(hash);
}