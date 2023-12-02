#include <ft_ssl.h>
#include <permutations.h>
#include <sboxes.h>

static uint64_t* create_subkeys(uint64_t key)
{
  uint32_t keys[17][2];
  uint32_t tmp;
  uint64_t* ret;
  int s;
  int n;

  ret = (uint64_t*)malloc(16 * sizeof(uint64_t));
  if(!ret)
    return(NULL);

  key = pbox(key, PC1, 64, 56);
  
  keys[0][LEFT] = key >> 28;
  keys[0][RIGHT] = key & ~(0xf << 28);

  for(int i = 1; i < 17; i++)
  {
    if(i == 1 || i == 2 || i == 9 || i == 16)
    {
      s = 1;
      n = 0b01;
    }
    else
    {
      s = 2;
      n = 0b11;
    }
    for(int j = 0; j < 2; j++)
    {
      tmp = left_rotation(keys[i-1][j] << 4, s);
      keys[i][j] = (tmp >> 4) | (tmp & n);
    }
    ret[i-1] = ((uint64_t)keys[i][LEFT]) << 28 | keys[i][RIGHT];
  }
  for(int i = 0; i < 16; i++)
    ret[i] = pbox(ret[i], PC2, 56, 48);
  return(ret);
}

static uint64_t feistel_struct(uint64_t plain, uint64_t* sub_keys, int dir)
{
  uint64_t permuted;
  uint32_t block[17][2];
  uint32_t sb = 0;
  uint64_t tmp;
  uint8_t b;
  uint8_t row;
  uint8_t col;

  permuted = pbox(plain, IP, 64, 64);

  block[0][LEFT] = permuted >> 32;
  block[0][RIGHT] = permuted & ~0;

  for(int i = 1; i < 17; i++)
  {
    block[i][LEFT] = block[i-1][RIGHT];
    tmp = pbox(block[i-1][RIGHT], E, 32, 48) ^ sub_keys[ft_abs(dir - (i-1))];
    for(int j = 0; j < 8; j++)
    {
      b = (tmp >> (48 - ((j+1) * 6))) & 0x3f;
      row = ((b >> 5) << 1 ) | (b & 1);
      col = (b >> 1) & 0xf;
      sb <<= 4;
      sb |= S[j][row][col];
    }
    block[i][RIGHT] = block[i-1][LEFT] ^ pbox(sb, P, 32, 32);
  }
  tmp = ((uint64_t)block[16][RIGHT]) << 32 | block[16][LEFT];
  return(pbox(tmp, FP, 64, 64));

}

uint64_t* des(uint8_t* argv, size_t len, uint8_t* key, uint8_t* initial_vector, int dir)
{
  uint64_t plain;
  uint64_t cypher;
  uint64_t key64;
  uint64_t iv_in;
  uint64_t o_plain;
  uint64_t iv_out;
  uint64_t* sub_keys;
  uint64_t* ret;
  uint64_t* ret_0;
  int mode = dir;
  if(dir == DES_PCBC_ENCRYPT || dir == DES_PCBC_DECRYPT)
    dir--;

  ret = (uint64_t*)malloc(len * sizeof(uint64_t));
  if(!ret)
    return(NULL);

  if(dir == DES_ENCRYPT)
  {
    iv_in = heap8_stack64(initial_vector);
    iv_out = 0x0;
  }
  else if(dir == DES_DECRYPT)
  {
    iv_in = 0x0;
    iv_out = heap8_stack64(initial_vector);
  }

  key64 = heap8_stack64(key);
  sub_keys = create_subkeys(key64);

  ret_0 = ret;
  while (len)
  {
    plain = heap8_stack64(argv);
    o_plain = plain;

    plain ^= iv_in;

    cypher = feistel_struct(plain, sub_keys, dir);

    cypher ^= iv_out;

    if(iv_in && !iv_out)
      iv_in = cypher;
    else if(!iv_in && iv_out)
      iv_out = plain;

    if(mode == DES_PCBC_ENCRYPT)
      iv_in ^= o_plain;
    else if(mode == DES_PCBC_DECRYPT)
      iv_out ^= cypher;

    argv += 8;
    if(len < 8)
      len = 0;
    else
      len -= 8;
    *ret = ft_bswap64(cypher);
    ret++;
  }

  free(sub_keys);
  return(ret_0);
}

uint64_t* des_bonus(uint8_t* argv, size_t len, uint8_t* key, uint8_t* initial_vector, int mode)
{
  uint64_t plain;
  uint64_t cypher;
  uint64_t key64;
  uint64_t iv_out;
  uint64_t* sub_keys;
  uint64_t* ret;
  uint64_t* ret_0;

  ret = (uint64_t*)malloc(len * sizeof(uint64_t));
  if(!ret)
    return(NULL);
  
  key64 = heap8_stack64(key);
  sub_keys = create_subkeys(key64);

  ret_0 = ret;
  plain = heap8_stack64(initial_vector);
  while (len)
  {

    cypher = feistel_struct(plain, sub_keys, DES_ENCRYPT);

    iv_out = heap8_stack64(argv);

    if((mode & 0b01110001) == DES_CFB)
    {
      cypher ^= iv_out;
      plain = cypher;

    }
    else if((mode & 0b01110001) == DES_CFB_DECRYPT)
    {
      cypher ^= iv_out;
      plain = iv_out;
    }
    else if((mode & 0b01110000) == DES_OFB)
    {
      plain = cypher;
      cypher ^= iv_out;
    }
    else if((mode & 0b01110000) == DES_CTR)
    {
      cypher ^= iv_out;
      plain++;
    }
    else
    {
      cypher ^= iv_out;
      initial_vector += 8;
      plain = heap8_stack64(initial_vector);
    }

    argv += 8;
    if(len < 8)
      len = 0;
    else
      len -= 8;
    *ret = ft_bswap64(cypher);
    ret++;
  }

  free(sub_keys);
  return(ret_0);
}