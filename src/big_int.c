#include<ft_ssl.h>

t_bigint* init_bigint(uint64_t n)
{
    t_bigint* h = (t_bigint*)malloc(sizeof(t_bigint));
    h->n = 1;
    h->len = (uint64_t*)malloc(h->n * sizeof(uint64_t));
    h->len[0] = n;
    h->sign = false;
    return(h);
}

t_bigint* copy_bigint(t_bigint* h)
{
    t_bigint* c = (t_bigint*)malloc(sizeof(t_bigint));
    c->n = h->n;
    c->sign = h->sign;
    uint64_t* tmp = (uint64_t*)malloc(h->n * sizeof(uint64_t));
    for (size_t i = 0; i < h->n; i++)
       tmp[i] = h->len[i];
    c->len = tmp;
    return(c);
}

void free_bigint(t_bigint* h)
{
    free(h->len);
    free(h);
}

void print_bigint(int fd, t_bigint* h, int base)
{
    uint64_t e;
    uint64_t z;
    uint64_t g;
    uint64_t* f;
    size_t size = 0;
    size_t st_len = 0;
    uint64_t ctrl = false;

    if(h->sign)
        write(fd, "-", 1);

    f = (uint64_t*)malloc(h->n * sizeof(uint64_t));
    for (size_t i = 0; i < h->n; i++)
    {
        e = h->len[i];
        f[i] = 0;
        z = e;
        if(ctrl)
            z = ~0UL;
        while (z)
        {
            f[i] <<= 4;
            f[i] |= e % base;
            e /=base;
            z >>= 4;
            size++;
            if(ctrl == false)
                st_len++;
        }
        if(h->len[i])
            ctrl = true;
    }
    ctrl=false;
    for (size_t i = 0; i < h->n; i++)
    {
        z = f[i];
        e = f[i];
        if(ctrl)
            z = ~0UL;
        while (z)
        {
            g = (e & 0xf) + '0';
            if(g >= 58)
                g += 7;
            write(fd, &g, 1);
            z >>= 4;
            e >>= 4;
            if(ctrl == false)
            {
                st_len--;
                z = st_len;
            }
        }
        if(f[i])
            ctrl = true;
    }
    free(f);
}

void bigint_inc_size(t_bigint* h, size_t n)
{
    h->n += n;
    uint64_t* tmp = (uint64_t*)malloc(h->n * sizeof(uint64_t));
    tmp[0] = 0;
    for (size_t i = 0; i < n; i++)
        tmp[i] = 0;
    for (size_t i = n; i < h->n; i++)
        tmp[i] = h->len[i-n];
    free(h->len);
    h->len = tmp;
}

void bigint_dec_size(t_bigint* h)
{
    size_t blocks = h->n;
    size_t i = 0;
    for (; i < h->n-1; i++)
    {
        if(h->len[i])
            break;
        blocks--;
    }
    if(blocks < h->n)
    {
        if(h->len[i] & (1UL << 63))
        {
            blocks++;
            i--;
        }
        uint64_t* tmp = (uint64_t*)malloc(sizeof(uint64_t) * blocks);
        for (size_t j = 0; j < blocks; j++)
            tmp[j] = h->len[i+j];
        h->n = blocks;
        free(h->len);
        h->len = tmp;
    }
}

void bigint_add_int(t_bigint* h, uint64_t d)
{
    h->len[h->n-1] = addition_modulus_uint64max(h->len[h->n-1], d);
    if(h->len[h->n-1] < d)
    {
        h->len[h->n-2] = addition_modulus_uint64max(h->len[h->n-2], 1);
        for (size_t i = 2; i < h->n; i++)
        {
            if(h->len[h->n-i] == 0)
                h->len[h->n-i-1] = addition_modulus_uint64max(h->len[h->n-i-1], 1);
            else
                break;
        }
    }
    
    if(h->len[0] & (0x1UL << 63))
        bigint_inc_size(h, 1);
}

void bigint_sub_int(t_bigint* h, uint64_t d)
{
        uint64_t tmp;
        uint64_t borrow = h->len[h->n-1] < d;
        if(borrow && (h->n == 1 || (h->n == 2 && h->len[0] == 0)))
        {
            h->sign = true;
            h->len[h->n-1] = d - h->len[h->n-1];
        }
        else
        {
            if(borrow)
            {
                tmp = (~0UL - d) + 1;
                h->len[h->n-1] = addition_modulus_uint64max(h->len[h->n-1], tmp);
            }
            else
               h->len[h->n-1] = h->len[h->n-1] - d;
            for (size_t i = 2; i <= h->n; i++)
            {
                if(borrow)
                {
                    tmp = ~0UL;
                    h->len[h->n-i] = addition_modulus_uint64max(h->len[h->n-i], tmp);
                }
                else
                {
                    h->len[h->n-i] -= 1;
                    break;
                }
                borrow = h->len[h->n-i] < borrow;
            }
        }
        bigint_dec_size(h);
}

void bigint_left_shift(t_bigint* h, size_t bits)
{
    uint64_t n;
    uint64_t carry;
    uint64_t tmp;
    size_t i = 0;

    if(bits == 0)
        return;
    while(bits > 64)
    {
        bits -= 64;
        i++;
    }
    
    carry = h->len[0] >> (64 - bits);
    if(carry)
        i++;
    if(i)
    {
        bigint_inc_size(h, i);
        if(carry)
            i--;
        for (size_t j = h->n; j > i && i > 0; j--)
        {
            h->len[h->n-j] = h->len[h->n-(j-i)];
            h->len[h->n-(j-i)] = 0;
        }
    }
    
    carry = 0;
    n = h->n;
    while(n--)
    {
        tmp = h->len[n] >> (64 - bits);
        if(bits >= 64)
            h->len[n] = 0;
        else
            h->len[n] <<= bits;
        h->len[n] |= carry;
        carry = tmp;
    }
}

void bigint_right_shift(t_bigint* h, size_t bits)
{
    uint64_t n;
    uint64_t carry;
    uint64_t tmp;
    int i = 0;

    if(bits == 0)
        return;
    while(bits >= 64)
    {
        bits -= 64;
        i++;
    }
    if(i)
    {
        for (size_t j = 1; j <= h->n - i; j++)
        {
            h->len[h->n-j] = h->len[h->n-j-i];
            h->len[h->n-j-i] = 0;
        }
        bigint_dec_size(h);
    }

    carry = 0;
    n = 0;
    while(n < h->n)
    {
        tmp = h->len[n] & ~(~0UL << bits);
        h->len[n] >>= bits;
        h->len[n] |= carry;
        carry = tmp << (64 - bits);
        n++;
    }
}

t_bigint* bigint_and(t_bigint* a, t_bigint* b)
{
    t_bigint* greater;
    t_bigint* lesser;
    size_t n;

    if(bigint_compare(a,b) < 0)
    {
        lesser = a;
        n = b->n;
        greater = copy_bigint(b);
    }
    else
    {
        lesser = b;
        n = a->n;
        greater = copy_bigint(a);
    }
    
    for (size_t i = 1; i <= n; i++)
    {
        if(i <= lesser->n)
        {
            greater->len[greater->n-i] &= lesser->len[lesser->n-i];
        }
        else
            greater->len[greater->n-i] = 0;
    }
    bigint_dec_size(greater);
    return(greater);
}

t_bigint* bigint_xor(t_bigint* a, t_bigint* b)
{
    t_bigint* greater;
    t_bigint* lesser;
    size_t n;

    if(bigint_compare(a,b) < 0)
    {
        lesser = a;
        n = b->n;
        greater = copy_bigint(b);
    }
    else
    {
        lesser = b;
        n = a->n;
        greater = copy_bigint(a);
    }
    
    for (size_t i = 1; i <= n; i++)
    {
        if(i <= lesser->n)
            greater->len[greater->n-i] ^= lesser->len[lesser->n-i];
    }
    bigint_dec_size(greater);
    return(greater);
}

int bigint_compare(t_bigint* a, t_bigint* b)
{
    size_t a_n;
    size_t b_n;
    size_t n;
    int ret;

    a_n = a->n;
    b_n = b->n;

    if(a->sign && !b->sign)
        return(-1);
    else if(!a->sign && b->sign)
        return(1);
    if(a->sign && b->sign)
        ret = -1;
    else
        ret = 1;

    while (a_n && a->len[a->n - a_n] == 0)
        a_n--;
    while (b_n && b->len[b->n - b_n] == 0)
        b_n--;
    if(a_n > b_n)
        return(ret);
    else if(a_n < b_n)
        return(-ret);
    n = a_n;
    while(n)
    {
        if(a->len[a->n - n] > b->len[b->n - n])
            return(ret);
        else if(a->len[a->n - n] < b->len[b->n - n])
            return(-ret);
        n--;
    }
    return(0);
}

t_bigint* bigint_add(t_bigint* a, t_bigint* b)
{
    t_bigint* result;
    t_bigint* greater;
    t_bigint* lesser;
    uint64_t x;
    uint64_t y;
    uint64_t carry;
    uint64_t tmp;

    if(bigint_compare(a,b) < 0)
    {
        lesser = a;
        greater = b;
    }
    else
    {
        lesser = b;
        greater = a;
    }

    result = init_bigint(0);
    bigint_inc_size(result, greater->n + 1);

    if(a->sign && b->sign)
    {
        result->sign = true;
    }
    else if(a->sign || b->sign)
    {
        lesser->sign = false;
        t_bigint* ret = bigint_sub(greater, lesser);
        lesser->sign = true;
        free_bigint(result);
        return(ret);
    }
    carry = 0;
    for (size_t i = 1; i <= result->n; i++)
    {
        if(i <= greater->n)
            x = greater->len[greater->n - i];
        else
            x = 0;
        if(i <= lesser->n)
            y = lesser->len[lesser->n - i];
        else
            y = 0;
        tmp = addition_modulus_uint64max(x,y);
        result->len[result->n-i] = addition_modulus_uint64max(tmp,carry);
        carry = result->len[result->n-i] < x;

    }
    bigint_dec_size(result);
    return(result);
}

t_bigint* bigint_mul(t_bigint* a, t_bigint* b)
{
    t_bigint* p;
    t_bigint* greater;
    t_bigint* lesser;
    size_t k;
    size_t i;
    uint64_t g_x;
    uint64_t g_y;
    uint64_t l_x;
    uint64_t l_y;

    if(bigint_compare(a,b) < 0)
    {
        lesser = a;
        greater = b;
    }
    else
    {
        lesser = b;
        greater = a;
    }
    
    if(lesser->sign)
    {
        lesser->sign = false;
        t_bigint* ret = bigint_mul(greater, lesser);
        lesser->sign = true;
        ret->sign = true;
        return(ret);
    }

    p = init_bigint(0);

    bigint_inc_size(p, (a->n + b->n)-1);
    k = 1;
    i = 0;
    for (size_t l = 1; l <= p->n; l++)
    {
        if(i == greater->n)
            k++;
        if(i < greater->n)
            i++;
        for (size_t j = k; j <= lesser->n && j <= i; j++)
        {
            g_x =  greater->len[greater->n - (i+k-j)] >> 32;
            g_y =  greater->len[greater->n - (i+k-j)] & (~0UL >> 32);
            l_x =  lesser->len[lesser->n - j] >> 32;
            l_y =  lesser->len[lesser->n - j] & (~0UL >> 32);

            uint64_t cis_y = l_y * g_y;
            uint64_t trans_y = l_y * g_x;

            uint64_t cis_x = l_x * g_x;
            uint64_t trans_x = l_x * g_y;

            p->len[p->n - l] = addition_modulus_uint64max(p->len[p->n - l], cis_y);
            if(p->len[p->n - l] < cis_y)
                p->len[p->n - l-1] = addition_modulus_uint64max(p->len[p->n - l-1], 1);

            p->len[p->n - l] = addition_modulus_uint64max(p->len[p->n - l], trans_y << 32);
            if(p->len[p->n - l] < (trans_y << 32))
                p->len[p->n - l-1] = addition_modulus_uint64max(p->len[p->n - l-1], 1);

            p->len[p->n - l] = addition_modulus_uint64max(p->len[p->n - l], trans_x << 32);
            if(p->len[p->n - l] < (trans_x << 32))
                p->len[p->n - l-1] = addition_modulus_uint64max(p->len[p->n - l-1], 1);

            p->len[p->n - l-1] = addition_modulus_uint64max(p->len[p->n - l-1], trans_y >> 32);
            if(p->len[p->n - l-1] < (trans_y >> 32))
                p->len[p->n - l-2] = addition_modulus_uint64max(p->len[p->n - l-2], 1);

            p->len[p->n - l-1] = addition_modulus_uint64max(p->len[p->n - l-1], trans_x >> 32);
            if(p->len[p->n - l-1] < (trans_x >> 32))
                p->len[p->n - l-2] = addition_modulus_uint64max(p->len[p->n - l-2], 1);

            p->len[p->n - l-1] = addition_modulus_uint64max(p->len[p->n - l-1], cis_x);
            if(p->len[p->n - l-1] < (cis_x))
                p->len[p->n - l-2] = addition_modulus_uint64max(p->len[p->n - l-2], 1);
        }
    }
    return(p);
}

t_bigint* bigint_sub(t_bigint* a, t_bigint* b)
{
    t_bigint* greater;
    t_bigint* lesser;
    t_bigint* result;
    uint64_t x;
    uint64_t y;
    uint64_t borrow;
    int comp;

    comp = bigint_compare(a, b);

    if(comp == 0)
        return(init_bigint(0));

    if(comp < 0)
    {
        lesser = copy_bigint(a);
        greater = (b);
    }
    else
    {
        lesser = copy_bigint(b);
        greater = (a);
    }

    if(a->sign && b->sign)
    {
        lesser->sign = false;
        greater->sign = false;
        t_bigint* ret;
        if(comp == 1)
            ret = bigint_sub(lesser, greater);
        else
            ret = bigint_sub(greater, lesser);
        free_bigint(lesser);
        lesser->sign = true;
        greater->sign = true;
        return(ret);
    }
    else if(a->sign || b->sign)
    {
        lesser->sign = false;
        t_bigint* ret = bigint_add(greater, lesser);
        if(a->sign)
            ret->sign = true;
        free_bigint(lesser);
        // lesser->sign = true;
        return(ret);
    }

    result = init_bigint(0);
    bigint_inc_size(result, greater->n-1);

    borrow = 0;
    for (size_t i = 1; i <= result->n; i++)
    {
        x = greater->len[greater->n - i];
        if(i <= lesser->n)
            y = lesser->len[lesser->n - i];
        else
            y = 0;
        if(borrow)
        {
            if (x == 0)
            {
                x = ~x;
                borrow = true;

            }
            else
            {
                x -=1;
                borrow = x < y;
            }
        }
        else
            borrow = x < y;
        if(borrow)
        {
            result->len[result->n-i] = (~0UL - y) + 1;
            result->len[result->n-i] = addition_modulus_uint64max(result->len[result->n-i], x);
        }
        else
           result->len[result->n-i] = x - y;
    }
    if(comp < 0)
        result->sign = true;
    free_bigint(lesser);
    bigint_dec_size(result);
    return(result);
}

t_bigint* bigint_div(t_bigint* a, t_bigint* b, t_bigint** r)
{
    t_bigint* quotient;
    t_bigint* remainder;
    t_bigint* divisor;
    uint64_t dividend;
    uint64_t cursor;
    t_bigint* tmp;
    size_t n;
    bool ctrl;

    if(a->sign ^ b->sign)
    {
        if(a->sign)
            tmp = a;
        else
            tmp = b;
        tmp->sign = false;
        quotient = bigint_div(a,b,r);
        quotient->sign = true;
        tmp->sign = true;
        return(quotient);
    }

    quotient = init_bigint(0);
    remainder = init_bigint(0);
    divisor = b;
    n = 0;
    dividend = a->len[n];
    cursor = 1UL << 63;

    if(bigint_compare(divisor, quotient) == 0)
    {
        free_bigint(quotient);
        free_bigint(remainder);
        return(NULL);
    }

    while(1)
    {
        ctrl = false;
        if(dividend & cursor)
            bigint_add_int(remainder, 1);
        if(bigint_compare(remainder, divisor) >= 0)
        {
            tmp = bigint_sub(remainder, divisor);
            free_bigint(remainder);
            remainder = tmp;
            ctrl = true;
        }
        bigint_left_shift(quotient, 1);
        if(ctrl)
            bigint_add_int(quotient, 1);
        cursor >>= 1;
        if(cursor == 0)
        {
            n++;
            if(n >= a->n)
                break;
            dividend = a->len[n];
            cursor = 1UL << 63;
        }
        bigint_left_shift(remainder, 1);
    }
    if(r)
        *r = remainder;
    else
        free_bigint(remainder);
    return(quotient);
}

t_bigint* bigint_sqrt(t_bigint* x)
{
    t_bigint* two = init_bigint(2);
    if (bigint_compare(x, two) < 0)
        return(x);
    free_bigint(two);

    t_bigint* sqr;
    t_bigint* mid;
    t_bigint* result = NULL;
    t_bigint* one = init_bigint(1);
    t_bigint* start = init_bigint(1);
    t_bigint* end = copy_bigint(x);
    bigint_right_shift(end,1);
 
    while (bigint_compare(start, end) <= 0)
    {
        mid = bigint_add(start, end);
        bigint_right_shift(mid,1);
        sqr = bigint_mul(mid,mid);
        if (bigint_compare(sqr, x) == 0)
        {
            free_bigint(sqr);
            free_bigint(one);
            free_bigint(start);
            free_bigint(end);
            if(result)
                free_bigint(result);
            return(mid);
        }
        else if(bigint_compare(sqr, x) < 0)
        {
            free_bigint(start);
            start = bigint_add(mid,one);
            if(result)
                free_bigint(result);
            result = mid;
        }
        else
        {
            free_bigint(end);
            end = bigint_sub(mid,one);
            free_bigint(mid);
        }
        free_bigint(sqr);
    }
    free_bigint(one);
    free_bigint(start);
    free_bigint(end);
    return(result);
}