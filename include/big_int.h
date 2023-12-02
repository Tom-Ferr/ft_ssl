#ifndef bigint_H
# define bigint_H


#include <stdint.h>
#include <unistd.h>

typedef struct s_bigint
{
    uint64_t n;
    uint64_t* len;
    bool sign;
}t_bigint;

typedef struct s_montgomery
{
    t_bigint* r;
    t_bigint* r_inv;
    t_bigint* mod_inv;
    t_bigint* k;
    t_bigint* mask;
    t_bigint* one_r;
    size_t    r_bits;
}t_montgomery;

t_bigint* init_bigint(uint64_t n);
t_bigint* copy_bigint(t_bigint* h);
void free_bigint(t_bigint* h);
void print_bigint(int fd, t_bigint* h, int base);
void bigint_dec_size(t_bigint* h);
void bigint_inc_size(t_bigint* h, size_t n);
void bigint_add_int(t_bigint* h, uint64_t d);
void bigint_sub_int(t_bigint* h, uint64_t d);
void bigint_left_shift(t_bigint* h, size_t bits);
void bigint_right_shift(t_bigint* h, size_t bits);
t_bigint* bigint_and(t_bigint* a, t_bigint* b);
t_bigint* bigint_xor(t_bigint* a, t_bigint* b);
int bigint_compare(t_bigint* a, t_bigint* b);
t_bigint* bigint_add(t_bigint* a, t_bigint* b);
t_bigint* bigint_mul(t_bigint* a, t_bigint* b);
t_bigint* bigint_sub(t_bigint* a, t_bigint* b);
t_bigint* bigint_div(t_bigint* a, t_bigint* b, t_bigint** r);
t_bigint* bigint_sqrt(t_bigint* x);

int bigint_miller_rabin(t_bigint* n, uint8_t p, bool print);

t_bigint* bigint_pow_mod(t_bigint* a_0, t_bigint* n_0, t_bigint* mod);
t_bigint* bigint_mul_mod(t_bigint* a_0, t_bigint* b_0, t_bigint* mod);
t_bigint* bigint_gcd(t_bigint* a_0, t_bigint* m_0);
t_bigint* bigint_inverse_mod_mul(t_bigint* a_0, t_bigint* m_0);
t_bigint* montgomery_mul(t_bigint* _a, t_bigint* _b, t_montgomery* m, t_bigint* mod);
t_bigint* montgomery_pow(t_bigint* a_0, t_bigint* n_0, t_bigint* mod, t_montgomery* m);
t_montgomery* init_montgomery(t_bigint* mod);
void free_montgomery(t_montgomery* m);


t_bigint* string_to_bigint(char* string, size_t len);
size_t bigint_to_string(uint8_t** str, t_bigint* bg);

#endif