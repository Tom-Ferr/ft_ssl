#include <ft_ssl.h>

uint8_t* oaep_encode(
            uint8_t* in, 
            size_t in_bytes, 
            uint8_t* p, 
            size_t out_bytes,
            uint8_t* H(uint8_t*, uint64_t), 
            size_t h_bytes)
{
    uint8_t* out;
    uint8_t* db;
    uint8_t* seed;
    size_t db_bytes;
    size_t ps_bytes;

    if(in_bytes > out_bytes - (2 * h_bytes) - 1)
        return(NULL);
    ps_bytes = out_bytes - in_bytes - (2 * h_bytes) - 1;
    db_bytes = h_bytes + ps_bytes + 1 + in_bytes;

    db = (uint8_t*)malloc(db_bytes);

    /*
    build DB
    */
    uint8_t* p_hash = H(p, ft_veclen(p));

    for (size_t i = 0; i < h_bytes; i++)
        db[i] = p_hash[i];
    free(p_hash);

    for (size_t i = 0; i < ps_bytes; i++)
        db[h_bytes + i] = 0x0;
    
    db[h_bytes + ps_bytes] = 0x1;

    for (size_t i = 0; i < in_bytes; i++)
        db[h_bytes + ps_bytes + 1 + i] = in[i];
    
    /*
    build Seed
    */
    seed = (uint8_t*)malloc(h_bytes);
    for (size_t i = 0; i < h_bytes; i++)
        seed[i] = (uint8_t)ft_urand();
    
    uint8_t* db_mask = mgf1(seed, h_bytes, db_bytes, H, h_bytes);

    /*
    XOR's
    */
    for (size_t i = 0; i < db_bytes; i++)
        db_mask[i] ^= db[i];
    free(db);

    uint8_t* seed_mask = mgf1(db_mask, db_bytes, h_bytes, H, h_bytes);

    for (size_t i = 0; i < h_bytes; i++)
        seed_mask[i] ^= seed[i];
    free(seed);

    /*
    build EM
    */
    out = (uint8_t*)malloc(out_bytes);

    for (size_t i = 0; i < h_bytes; i++)
        out[i] = seed_mask[i];
    for (size_t i = 0; i < db_bytes; i++)
        out[h_bytes + i] = db_mask[i];
    
    free(seed_mask);
    free(db_mask);

    return(out);
     
}

uint8_t* oaep_decode(
            uint8_t* in, 
            size_t in_bytes, 
            uint8_t* p,
            uint8_t* H(uint8_t*, uint64_t), 
            size_t h_bytes)
{
    if(in_bytes < (2 * h_bytes) + 1)
        return(NULL);

    uint8_t* seed_mask;
    uint8_t* db_mask;
    uint8_t* p_hash;
    size_t  db_bytes;

    db_bytes = in_bytes - h_bytes;
    seed_mask = (uint8_t*)malloc(h_bytes);
    db_mask = (uint8_t*)malloc(db_bytes);

    for (size_t i = 0; i < h_bytes; i++)
        seed_mask[i] = in[i];
    for (size_t i = 0; i < in_bytes; i++)
        db_mask[i] = in[h_bytes + i];

    /*
    restore seed
    */
    uint8_t* seed = mgf1(db_mask, db_bytes, h_bytes, H, h_bytes);
    for (size_t i = 0; i < h_bytes; i++)
        seed[i] ^= seed_mask[i];
    free(seed_mask);

    /*
    restore db
    */
    uint8_t* db = mgf1(seed, h_bytes, db_bytes, H, h_bytes);
    for (size_t i = 0; i < db_bytes; i++)
        db[i] ^= db_mask[i];
    free(db_mask);

    p_hash = H(p, ft_veclen(p));

    /*
    comapare Phash
    */
    size_t i = 0;
    for (; i < h_bytes; i++)
    {
        if(db[i] != p_hash[i])
        {
            free(p_hash);
            free(seed);
            free(db);
            return(NULL);
        }
    }

    /*
    skip PS
    */
    free(p_hash);
    for (; i < db_bytes; i++)
    {
        if(db[i] == 0x1)
            break ;
    }
    i++;
    if(i >= db_bytes)
    {
        free(seed);
        free(db);
        return(NULL);
    }
    
    /*
    get message
    */
    uint8_t* out = (uint8_t*)malloc(db_bytes - i + 1);
    for (size_t j = 0; j < db_bytes - i; j++)
        out[j] = db[i + j];
    out[db_bytes - i] = '\0';
    
    free(db);
    free(seed);
    return(out);
    
}