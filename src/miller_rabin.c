#include <ft_ssl.h>

static uint64_t refactor(uint64_t n, uint64_t* m)
{
    uint64_t i;

    if(!n)
        return(0);

    i = 0;
    while ((n % 2) == 0)
    {
        n /= 2;
        *m = n;
        i++;
    }

    return(i);
}

int miller_rabin(uint64_t n, uint8_t p)
{
    uint64_t k;
    uint64_t m;
    uint64_t a;
    uint64_t b;
    uint64_t i;

    if(n == 2)
        return(-1);

    if(!(n & 1) || n == 1)
        return(0);

    k = refactor(n-1, &m);
    if(!k)
        return(0);
    while (p)
    {
        do  
            a = ft_urand() % (n-1);
        while (1 >= a);
    
        b = ft_pow_mod(a, m, n);

        if(b == (n - 1) || b == 1)
        {
            p--;
            continue;
        }

        i = k;
        while (b != (n - 1) && b != 1 && --i)
            b = ft_pow_mod(b, 2, n);
        if(b == 1 || !i)
            return(1);
        p--;
    }
    return(-1);
}