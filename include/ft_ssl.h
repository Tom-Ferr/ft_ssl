#ifndef FT_SSL_H
# define FT_SSL_H
# define L 0x100000000
# define LEFT 0
# define RIGHT 1
# define DES_ENCRYPT 0
# define DES_DECRYPT 15
# define DES_PCBC_ENCRYPT 1
# define DES_PCBC_DECRYPT 16
# define DES_CFB 16 
# define DES_CFB_DECRYPT 17
# define DES_OFB 32 
# define DES_CTR 64
# define AES_ENCRYPT 1
# define AES_DECRYPT -1
# define AES_PCBC_ENCRYPT 2
# define AES_PCBC_DECRYPT 0
# define AES_CFB 16 
# define AES_CFB_DECRYPT 17
# define AES_OFB 32 
# define AES_CTR 64
# define PRIVATE 16 
# define PUBLIC 4
# define RSA 16 
# define DSA 10


# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <stdint.h>
# include <math.h>
# include <stdbool.h>
# include <fcntl.h>
# include <big_int.h>
# include <options.h>
# include <termios.h>

typedef uint64_t (*t_nlp)(uint64_t, uint64_t, uint64_t);
typedef uint32_t* (*t_aes)(uint32_t*, uint32_t**);

typedef struct s_rsakey
{
    t_bigint* n;
    t_bigint* e;
    t_bigint* d;
    t_bigint* p;
    t_bigint* q;
    t_bigint* dp;
    t_bigint* dq;
    t_bigint* iq;
} t_rsakey;

typedef struct s_dsakey
{
    t_bigint* p;
    t_bigint* q;
    t_bigint* g;
    t_bigint* y;
    t_bigint* x;
} t_dsakey;

typedef struct s_shawe_taylor
{
    t_bigint* prime;
    t_bigint* prime_seed;
    size_t prime_gen_counter;
} t_shawe_taylor;

typedef struct s_der
{
    uint8_t* n;
    size_t l_n;
    uint8_t* e;
    size_t l_e;
    uint8_t* d;
    size_t l_d;
    uint8_t* p;
    size_t l_p;
    uint8_t* q;
    size_t l_q;
    uint8_t* dp;
    size_t l_dp;
    uint8_t* dq;
    size_t l_dq;
    uint8_t* iq;
    size_t l_iq;
    size_t sequence_length;
    size_t size;
} t_der;

/*
Utils
*/
size_t	ft_strlen(const char *str);
size_t	ft_veclen(const uint8_t *vec);
void free_star(uint32_t **me);
void free_star_char(char **me);
int	ft_strcmp(char *str1, char *str2);
int	ft_strlcmp(char *str1, char *str2, size_t len);
uint32_t right_rotation(uint32_t x,uint32_t y);
uint32_t left_rotation(uint32_t x,uint32_t y);
uint64_t right_rotation_64(uint64_t x,uint64_t y);
uint64_t left_rotation_64(uint64_t x,uint64_t y);
uint64_t ft_bswap64(uint64_t value);
uint32_t ft_bswap32(uint32_t value);
int ft_rand(void);
uint64_t ft_urand(void);
off_t input_file(char* file, char** ret);
size_t read_stdin(char** ret, bool line);
uint32_t	mini_atoi(char argv[]);
char    **ft_split(char const *s, char c);
char* format_cmd(char* in, size_t size);
size_t ft_getpass(char** pass, int mode, char* name);
void error_message(char* cmd, char* opt, char* message);

/*
Hash
*/
uint64_t* add_padding(const uint8_t* plain_text, uint64_t* total_len, const uint64_t n_bits, const uint64_t length_field);
void bswap_length_field(uint64_t* text, size_t length_field);
uint32_t* divide_into_blocks(uint64_t* message);
uint64_t f_process(uint64_t x, uint64_t y, uint64_t z);
uint64_t g_process(uint64_t x, uint64_t y, uint64_t z);
uint64_t h_process(uint64_t x, uint64_t y, uint64_t z);
uint64_t i_process(uint64_t x, uint64_t y, uint64_t z);
uint32_t** shift_array(void);
uint32_t** constant_array(void);
uint64_t sigma1(uint64_t x);
uint64_t sigma2(uint64_t x);
uint64_t sigma1_512(uint64_t x);
uint64_t sigma2_512(uint64_t x);
uint64_t ch(uint64_t x,uint64_t y,uint64_t z);
uint64_t maj(uint64_t x,uint64_t y,uint64_t z);
void add_buffers(uint32_t* buffer, uint32_t* original, int size);
void copy_buffer(uint32_t* buffer, uint32_t* original, int size);
void update_buffer(uint32_t* buffer, uint32_t result, int size, int h);
void add_buffers_64(uint64_t* buffer, uint64_t* original, int size);
void copy_buffer_64(uint64_t* buffer, uint64_t* original, int size);
void update_buffer_64(uint64_t* buffer, uint64_t result, int size, int h);
uint8_t* md5(uint8_t* argv, uint64_t len);
uint8_t* sha1(uint8_t* argv, uint64_t len);
uint8_t* sha224(uint8_t* argv, uint64_t len);
uint8_t* sha256(uint8_t* argv, uint64_t len);
uint8_t* sha384(uint8_t* argv, uint64_t len);
uint8_t* sha512(uint8_t* argv, uint64_t len);
uint8_t* sha512_224(uint8_t* argv, uint64_t len);
uint8_t* sha512_256(uint8_t* argv, uint64_t len);
uint8_t* whirlpool(uint8_t* argv, uint64_t len);
void hash_options(uint8_t* H(uint8_t*, uint64_t), size_t h_bit, char** argv, int argc);
uint8_t* hash_to_vec(uint64_t* hash, size_t h_bytes);

/*
Math
*/
double ft_floor(double n);
double ft_ceil(double n);
double ft_fabs(double n);
double ft_sin(double n);
int ft_abs(int n);
uint64_t ft_pow(uint64_t a, uint64_t n);
uint64_t ft_pow_mod(uint64_t a, uint64_t n, uint64_t mod);
uint64_t ft_mul_mod(uint64_t a, uint64_t b, uint64_t mod);
long inverse_mod_mul(long a, long m);
uint64_t addition_modulus_uint64max(uint64_t a, uint64_t b);

/*
Print
*/
void ft_puthex(int fd, uint64_t d, bool a);
void put_int_base(int fd, uint64_t e, int base);
void print_hash(int fd, uint8_t* hash, uint32_t block_size);
size_t atox(uint8_t** vec, char* str);
void print_base64(int fd, char* text, size_t len);
void print_eval(int fd, t_bigint* e);
void print_rsakey(int fd, char* text, int vis, bool salted, char* iv);
void print_modulus(int fd, t_rsakey* k);
void print_key_values(int fd, t_der* der, int vis);
void printfy_int_base(uint8_t* vec, uint8_t e, int base);

/*
Base64
*/
char* base64_encode(uint8_t* argv, size_t len);
char* base64_decode(uint8_t* cypher, size_t len, size_t* out_size);
void base64_options(char** argv, int argc);

/*
DES
*/
uint64_t initial_permutation(uint64_t plain);
uint64_t final_permutation(uint64_t plain);
uint64_t pcOne(uint64_t plain);
uint64_t pbox(uint64_t plain, int* p, int input_size, int output_size);
uint64_t* des(uint8_t* argv, size_t len, uint8_t* key, uint8_t* initial_vector, int dir);
uint8_t* hmac(uint8_t* plain, size_t p_bytes, uint8_t* k, size_t k_bytes, uint8_t* (*H)(uint8_t*,uint64_t), size_t h_bytes, size_t block_size);
uint64_t heap8_stack64(uint8_t* heap);
uint32_t heap8_stack32(uint8_t* heap);
uint8_t* i64tov8(uint64_t stack);
uint8_t* parse_salted(char** src, size_t size);
uint8_t* pbdkf2(uint8_t* password, size_t p_len, uint8_t* salt, size_t c, size_t k_bytes, uint8_t* (*H)(uint8_t*, uint64_t), size_t h_bytes);
size_t pkcss5_padding(uint8_t** dest, uint8_t* src, int target);
size_t pkcss5_remove_pad(uint8_t** padded, size_t p_len);
uint64_t* des_bonus(uint8_t* argv, size_t len, uint8_t* key, uint8_t* initial_vector, int mode);
uint8_t* key_pad(uint8_t* k, size_t len, size_t block_size);
void des_options(char** argv, int argc);
void gendes_options(char** argv, int argc);

/*
RSA
*/
int miller_rabin(uint64_t n, uint8_t c);
int bigint_miller_rabin(t_bigint* n, uint8_t p, bool print);
t_bigint* prime_gen(int fd);
void rsa_keygen(t_rsakey* k);
t_der* key_to_der(void* key, int mode);
uint8_t* format_der(t_der* der, int vis);
size_t parse_pem(char** pem, char* buff, size_t size, int vis, bool* salted, char** iv);
t_der* decode_pem(char* pem, size_t len, int mode);
void parse_der(t_der* p, uint8_t* der, size_t size, int mode);
void* der_to_key(void* der, int mode);
uint8_t* extract_publickey(t_der* k);
void check_rsakey(t_rsakey* k);
uint8_t* mgf1(uint8_t* Z, size_t z_bytes, size_t len, uint8_t* (*H)(uint8_t*, uint64_t), size_t h_bytes);
uint8_t* oaep_encode(uint8_t* in, size_t in_bytes, uint8_t* p, size_t out_bytes, uint8_t* H(uint8_t*, uint64_t), size_t h_bytes);
uint8_t* oaep_decode(uint8_t* in, size_t in_bytes, uint8_t* p, uint8_t* H(uint8_t*, uint64_t), size_t h_bytes);
t_bigint* rsa_encode(uint8_t* m, size_t m_bytes, t_rsakey k);
t_bigint* rsa_decode(uint8_t* c, size_t c_bytes, t_rsakey k);
size_t key_to_string(uint8_t** str, t_rsakey key);
void free_key(void* k, int mode);
void free_der(t_der* der, size_t mode);
void genrsa_options(char** argv, int argc);
void rsa_options(char** argv, int argc);
void rsautl_options(char** argv, int argc);
size_t hex_dump(uint8_t** rsa_str, size_t size);
size_t getkey_bitsize(t_der* der);
int sieve(t_bigint* p, int bit_size);

/*
AES
*/
void aes_128_options(char** argv, int argc);
uint32_t* aes_128(uint8_t* argv, size_t len, uint8_t* key, uint8_t* initial_vector, int dir);
uint32_t* aes_128_bonus(uint8_t* argv, size_t len, uint8_t* key, uint8_t* initial_vector, int mode);

/*
DSA
*/
void gendsa_options(char** argv, int argc);
void dsa_options(char** argv, int argc);
t_dsakey* gen_dsa_primes(int l, int n, int seedlen);
void gen_dsa_generator(t_dsakey* params);
void gen_dsa_key(t_dsakey* key);
void free_shawe_taylor(t_shawe_taylor* c);
t_shawe_taylor* st_generate_random_prime(size_t length, t_shawe_taylor* key, t_bigint* salt);
t_shawe_taylor* shawe_taylor(size_t length, t_bigint* input_seed);


#endif