#include <ft_ssl.h>

static uint64_t s0(uint64_t w)
{
    return(right_rotation_64(w, 1) ^ right_rotation_64(w, 8) ^ (w >> 7));
}

static uint64_t s1(uint64_t w)
{
    return(right_rotation_64(w, 19) ^ right_rotation_64(w, 61) ^ (w >> 6));
}

static uint64_t* append_blocks(uint64_t* blocks, uint64_t len)
{
    uint64_t* w;
    uint64_t* w_0;
    uint64_t* blocks_0;
    int i;
    uint64_t n_chunks;

    n_chunks = (len * 8) / 1024;
    w = (uint64_t*)malloc((80 * n_chunks) * sizeof(uint64_t));
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

                w[i] = ft_bswap64(blocks[i]);
            }
            else
                w[i] = 0x0;
            i++;
        }

        i = 16;
        while (i < 80)
        {
            w[i] = addition_modulus_uint64max(w[i - 16], s0(w[i - 15])); 
            w[i] = addition_modulus_uint64max(w[i], w[i - 7]); 
            w[i] = addition_modulus_uint64max(w[i], s1(w[i - 2]));
            i++; 
        }
        n_chunks--;
        blocks += 16;
        w += 80;
    }
    free(blocks_0);
    return(w_0);
}

static uint64_t* init_buffers(void)
{
    uint64_t* buffers;

    buffers = (uint64_t*)malloc(8 * sizeof(uint64_t));
    if(!buffers)
        return(NULL);

    buffers[0] = 0x22312194FC2BF72C;
    buffers[1] = 0x9F555FA3C84C64C2;
    buffers[2] = 0x2393B86B6F53B151;
    buffers[3] = 0x963877195940EABD;
    buffers[4] = 0x96283EE2A88EFFE3;
    buffers[5] = 0xBE5E1E2553863992;
    buffers[6] = 0x2B0199FC2C85B8AA;
    buffers[7] = 0x0EB72DDC81C52CA2;

    return(buffers);

}

static uint8_t* digest(uint64_t* blocks, uint64_t* buffer, uint64_t n_chunks)
{
    int i;
    uint64_t result_1;
    uint64_t result_2;
    uint64_t k[80] = {
        0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
        0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 
        0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
        0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694, 
        0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
        0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
        0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4,
        0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
        0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
        0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
        0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30,
        0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
        0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8, 
        0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
        0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
        0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b, 
        0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
        0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
        0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c, 
        0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817
    };
    uint64_t original[8];

    while(n_chunks)
    {
        copy_buffer_64(buffer, original, 8);
        i = 0;
        while(i < 80)
        {
            result_1 = addition_modulus_uint64max(k[i], blocks[i]);
            result_1 = addition_modulus_uint64max(result_1, ch(buffer[4], buffer[5], buffer[6]));
            result_1 = addition_modulus_uint64max(result_1, sigma1_512(buffer[4]));
            result_1 = addition_modulus_uint64max(result_1, buffer[7]);
            buffer[3] = addition_modulus_uint64max(buffer[3], result_1);
            result_2 = addition_modulus_uint64max(sigma2_512(buffer[0]), maj(buffer[0], buffer[1], buffer[2]));
            update_buffer_64(buffer, result_2, 8,0);
            buffer[0] = addition_modulus_uint64max(buffer[0], result_1);
            i++;
        }
        add_buffers_64(buffer, original, 8);
        blocks += 80;
        n_chunks--;
    }


    i = 0;
    while (i < 8)
    {
        buffer[i] = ft_bswap64(buffer[i]);
        i++;
    }

    int hash_len = 256/8;
    uint8_t* hash = (uint8_t*)malloc(hash_len + 1);
    for (int i = 0; i < hash_len; i++)
        hash[i] = *((uint8_t*)buffer + i);
    hash[hash_len] = '\0';
    return (hash);
}

uint8_t* sha512_256(uint8_t* argv, uint64_t len)
{
    uint64_t* padded_text;
    uint64_t* buffer;
    uint64_t* blocks;
    uint8_t* hash;
    
    padded_text = add_padding(argv, &len, 128, 16);

    if(!padded_text)
        return(NULL);
    bswap_length_field(padded_text + ((len - 16) / 8), 16);
    buffer = init_buffers();
    if(!buffer)
        return(NULL);
    blocks = append_blocks(padded_text, len);

    hash = digest(blocks, buffer,  (len * 8) / 1024);

    free(blocks);
    free(buffer);
    return(hash);
}